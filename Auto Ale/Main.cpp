//####################                 Auto Fantastico              ####################

// Mejorado por Alejandro Hernández y Luciano Cendra


/*				[INDICE]

-Reproducción de Audio
-Dibujos a Pantalla
-Puntajes
-Disparos
-Desactivación
-Movimiento y Velocidad
-Control de Gas
-Salud y Vida
-Colisiones
-Inicialización
-Condiciones de carretera
-Sensores de Piloto Automático

-MAIN

*/

// Inclusion de las librerias
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <windows.h>
//**********************************************************

/*Inclusion de librerias creadas especificamente para este proyecto
Estas librerias son los structs correspondientes a cada uno de los personajes, balas o bonus
*/
#include "Auto.h" 
#include "EnemigoPentagono.h"
#include "EnemigoTriangulo.h"
#include "BalaEnemigo.h" 
#include "BalaPersonaje.h"
#include "Bonus.h"
#include "Carretera.h"

#include <allegro5/allegro.h>

#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>

#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>

#include <allegro5/allegro_image.h>

#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

using namespace std;
//**********************************************************

//Define parametros que se implementaran para este juego

//tamaño de la pantalla de interfaz de allegro
#define anchoPantalla 1100
#define largoPantalla 600

//posicion en la que se va a colocar la pantalla
#define posicionRectanguloJuego 80

//Definicion de timers que se utilizaran en el juego
#define FPS 30.0
#define FPS1 10.0
#define FPS2 15.0
#define FPS3 45.0
#define FPS4 1.0
#define FPS5 20.0
#define FPS6 20.0
#define FPS7 5.0
#define FPS8 0.1
#define FPS9 35.0
#define FPS10 0.5

//definicion de otros parametros necesarios en el juego
#define dannoBalaEnemigo 5
#define dannoEnemigoTriangulo 3.5
#define numeroEnemigosTriangulo 1
#define numeroturboPersonaje 100
#define numeroBalasPersonaje 100
#define numeroBalasEnemigo 5
#define numeroBonus 2
#define saludBonus 5
#define sumaPuntajeColision 50
#define sumaPuntajeTimer 25
#define restagastimer 2
#define sumagasusadotimer 2
#define sumaPuntajeBonusSalud 0.5
#define sumaPuntajeBonusVida 15
#define posicionInformacion  0
#define mejoresPuntajesJuego 10
//**********************************************************

//Globales

//definicion de las teclas utilizadas para el movimiento del personaje y para realizar los disparos
enum Direccion { W, S, D, A };
enum Teclas { J, I, L };
enum Piloto { M };

//Se definen como false al inicio las teclas, osea, que no han sido presionadas
bool teclasDireccion[4] = { false, false, false, false };
bool teclasDisparo[3] = { false, false, false };
bool teclaPiloto[1] = { false };

//Definicion de variables globales necesarias en el juego
int puntaje = 0;
int gas = 100;
int gasusado = 0;
float velocidad = 0.0;
int marcha = 1;
bool turb = false;
int balasdisp = 0;
int bonusop = 0;
int enemigosel = 0;
int balasperdidas = 0;
int numenemigos = 0;
int bonusgenerados = 0;
int bonusperdidos = 0;
int clima = 1;
int material = 1;
int pendiente = 1;

FILE *documento;
//********************************************************** 

//Define punteros  que apuntan hacia listas de structs 
EnemigoTriangulo *enemigosTriangulo[numeroEnemigosTriangulo];
BalaEnemigo *balasEnemigo[numeroBalasEnemigo];
BalaPersonaje *balasPersonaje[numeroBalasPersonaje];
Bonus *bonus[numeroBonus];
int mejoresPuntajes[mejoresPuntajesJuego];

//Se dedfine un puntero hacia un personaje principal
PersonajePrincipal *personaje;
//**********************************************************

//Elementos de allegro que se utilizarán para el juego

ALLEGRO_DISPLAY *pantalla;

ALLEGRO_COLOR transparente;

ALLEGRO_BITMAP *carretera1;
ALLEGRO_BITMAP *carretera2;

Carretera *ncarretera1;
Carretera *ncarretera2;

ALLEGRO_BITMAP *principalBuffer;
ALLEGRO_BITMAP *enemigoTrianguloBuffer;
//ALLEGRO_BITMAP *carretera;

ALLEGRO_BITMAP *principalIzquierda;
ALLEGRO_BITMAP *principalDerecha;
ALLEGRO_BITMAP *principalAturdidoIzquierda;
ALLEGRO_BITMAP *principalAturdidoDerecha;
ALLEGRO_BITMAP *principalCriticoIzquierda;
ALLEGRO_BITMAP *principalCriticoDerecha;
ALLEGRO_BITMAP *enemigoTriangulo;
ALLEGRO_BITMAP *bonusSalud;
ALLEGRO_BITMAP *bonusVida;
ALLEGRO_BITMAP *balaPersonajeH;
ALLEGRO_BITMAP *balaPersonajeV;
ALLEGRO_BITMAP *balaEnemigo;
ALLEGRO_BITMAP *ultimo;

ALLEGRO_SAMPLE *musicaJuego;
ALLEGRO_SAMPLE *disparoPersonaje;
ALLEGRO_SAMPLE *marchaCambia;
ALLEGRO_SAMPLE *voz_saludbaja;
ALLEGRO_SAMPLE *voz_gasbajo;
ALLEGRO_SAMPLE *voz_bajevel;
ALLEGRO_SAMPLE *turbPersonaje;
ALLEGRO_SAMPLE *disparoEnemigo;
ALLEGRO_SAMPLE *colisionEnemigo;

ALLEGRO_TIMER *timer;

ALLEGRO_SAMPLE_INSTANCE *instanciaSonido;

ALLEGRO_FONT *fuente;

//**********************************************************

//FUNCIONES NECESARIAS EN EL JUEGO





//	_________________________________________________________________________________________    REPRODUCCIÓN DE AUDIO



//reproducirMusicaJuego: función encargada de reproducir el tema principal del juego
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void reproducirMusicaJuego() {
	al_play_sample(musicaJuego, 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
}


//reproducirTurboPersonaje: función encargada de reproducir el sonido para el turbo del personaje
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void reproducirturboPersonaje() {
	instanciaSonido = al_create_sample_instance(turbPersonaje);
	al_set_sample_instance_playmode(instanciaSonido, ALLEGRO_PLAYMODE_ONCE);
	al_attach_sample_instance_to_mixer(instanciaSonido, al_get_default_mixer());
	al_play_sample_instance(instanciaSonido);
}


//reproducirDisparoEnemigo: función encargada de reproducir el sonido para el disparo del enemigo
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void reproducirDisparoEnemigo() {
	instanciaSonido = al_create_sample_instance(disparoEnemigo);
	al_set_sample_instance_playmode(instanciaSonido, ALLEGRO_PLAYMODE_ONCE);
	al_attach_sample_instance_to_mixer(instanciaSonido, al_get_default_mixer());
	al_play_sample_instance(instanciaSonido);
}


//reproducirColisionEnemigo: función encargada de reproducir el sonido para la colisión entre el enemigo y la bala del personaje
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void reproducirColisionEnemigo() {
	instanciaSonido = al_create_sample_instance(colisionEnemigo);
	al_set_sample_instance_playmode(instanciaSonido, ALLEGRO_PLAYMODE_ONCE);
	al_attach_sample_instance_to_mixer(instanciaSonido, al_get_default_mixer());
	al_play_sample_instance(instanciaSonido);
}

//reproducirDisparoPersonaje: función encargada de reproducir el sonido para el disparo del personaje
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void reproducirDisparoPersonaje() {
	instanciaSonido = al_create_sample_instance(disparoPersonaje);
	al_set_sample_instance_playmode(instanciaSonido, ALLEGRO_PLAYMODE_ONCE);
	al_attach_sample_instance_to_mixer(instanciaSonido, al_get_default_mixer());
	al_play_sample_instance(instanciaSonido);
}

//reproducirCambioMarcha: función encargada de reproducir el sonido para el disparo del personaje
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void reproducirCambioMarcha() {
	instanciaSonido = al_create_sample_instance(marchaCambia);
	al_set_sample_instance_playmode(instanciaSonido, ALLEGRO_PLAYMODE_ONCE);
	al_attach_sample_instance_to_mixer(instanciaSonido, al_get_default_mixer());
	al_play_sample_instance(instanciaSonido);
}

//reproducirSaludBaja: función encargada de reproducir el sonido de voz del dash: salud baja
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void reproducirSaludBaja() {
	if (personaje->salud <= 15) {
		instanciaSonido = al_create_sample_instance(voz_saludbaja);
		al_set_sample_instance_playmode(instanciaSonido, ALLEGRO_PLAYMODE_ONCE);
		al_attach_sample_instance_to_mixer(instanciaSonido, al_get_default_mixer());
		al_play_sample_instance(instanciaSonido);
	}
}

//reproducirGasBajo: función encargada de reproducir el sonido de voz del dash: salud baja
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void reproducirGasBajo() {
	if (gas <= 15) {
		instanciaSonido = al_create_sample_instance(voz_gasbajo);
		al_set_sample_instance_playmode(instanciaSonido, ALLEGRO_PLAYMODE_ONCE);
		al_attach_sample_instance_to_mixer(instanciaSonido, al_get_default_mixer());
		al_play_sample_instance(instanciaSonido);
	}
}

//reproducirBajeVel: función encargada de reproducir el sonido de voz del dash: baje la velocidad
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void reproducirBajeVel() {
	if (velocidad == 300) {
		instanciaSonido = al_create_sample_instance(voz_bajevel);
		al_set_sample_instance_playmode(instanciaSonido, ALLEGRO_PLAYMODE_ONCE);
		al_attach_sample_instance_to_mixer(instanciaSonido, al_get_default_mixer());
		al_play_sample_instance(instanciaSonido);
	}

}

//	_________________________________________________________________________________________			DIBUJOS A PANTALLA






//dibujarMenu: función encargada de dibujar en pantalla las opciones disponibles del menu del juego
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void dibujarMenu() {
	al_draw_text(fuente, al_map_rgb(255, 255, 255), 600, 150, ALLEGRO_ALIGN_CENTRE, "Selccione uno de los siguientes");
	al_draw_text(fuente, al_map_rgb(255, 255, 255), 600, 260, ALLEGRO_ALIGN_CENTRE, "1 -  JUGAR");
	al_draw_text(fuente, al_map_rgb(255, 255, 255), 600, 320, ALLEGRO_ALIGN_CENTRE, "2 -  VER TABLA DE PUNTAJES");
	al_draw_text(fuente, al_map_rgb(255, 255, 255), 600, 380, ALLEGRO_ALIGN_CENTRE, "3 -  SALIR");
	al_flip_display();
}


//dibujarPuntaje: función encargada de dibujar en pantalla el puntaje actual
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void dibujarPuntaje() {
	char puntajeCadena[10];
	sprintf(puntajeCadena, "%i""%", puntaje);
	al_draw_text(fuente, al_map_rgb(255, 0, 0), 100, 500, ALLEGRO_ALIGN_CENTRE, "Puntaje");
	al_draw_text(fuente, al_map_rgb(255, 255, 255), 100, 530, ALLEGRO_ALIGN_CENTRE, puntajeCadena);
	al_flip_display();
}

//dibujargas: función encargada de dibujar en pantalla la gasolina restante
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void dibujargas() {
	char gasCadena[10];
	sprintf(gasCadena, "%i""%", gas);
	al_draw_text(fuente, al_map_rgb(255, 0, 0), 100, 345, ALLEGRO_ALIGN_CENTRE, "Gas");
	al_draw_text(fuente, al_map_rgb(255, 255, 255), 100, 375, ALLEGRO_ALIGN_CENTRE, gasCadena);
	al_flip_display();
}


//dibujar controles del juego: función encargada de dibujar en pantalla los controles del juego
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void dibujarcontroles() {
	al_draw_text(fuente, al_map_rgb(0, 0, 255), 1000, 50, ALLEGRO_ALIGN_CENTER, "Manejo");
	al_draw_text(fuente, al_map_rgb(255, 255, 255), 1000, 80, ALLEGRO_ALIGN_CENTER, "A  W  S  D");
	al_draw_text(fuente, al_map_rgb(0, 0, 255), 1000, 130, ALLEGRO_ALIGN_CENTER, "Disparo");
	al_draw_text(fuente, al_map_rgb(255, 255, 255), 1000, 160, ALLEGRO_ALIGN_CENTER, "J  I  L");
	al_draw_text(fuente, al_map_rgb(0, 0, 255), 1000, 210, ALLEGRO_ALIGN_CENTER, "Piloto");
	al_draw_text(fuente, al_map_rgb(0, 0, 255), 1000, 240, ALLEGRO_ALIGN_CENTER, "Automatico");
	al_draw_text(fuente, al_map_rgb(255, 255, 255), 1000, 270, ALLEGRO_ALIGN_CENTER, "M");
	al_flip_display();
}

//dibujarCondiciones: función encargada de dibujar en pantalla las condiciones de la carretera: pendiente, material y clima
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void dibujarCondiciones() {

	al_draw_text(fuente, al_map_rgb(0, 0, 255), 1000, 320, ALLEGRO_ALIGN_CENTRE, "Clima");
	if (clima == 1) {
		al_draw_text(fuente, al_map_rgb(255, 255, 255), 1000, 350, ALLEGRO_ALIGN_CENTRE, "Soleado");
	}
	else if (clima == 2) {
		al_draw_text(fuente, al_map_rgb(255, 255, 255), 1000, 350, ALLEGRO_ALIGN_CENTRE, "Lluvioso");
	}
	else {
		al_draw_text(fuente, al_map_rgb(255, 255, 255), 1000, 350, ALLEGRO_ALIGN_CENTRE, "Nevado");
	}

	al_draw_text(fuente, al_map_rgb(0, 0, 255), 1000, 400, ALLEGRO_ALIGN_CENTRE, "Material");
	if (material == 1) {
		al_draw_text(fuente, al_map_rgb(255, 255, 255), 1000, 430, ALLEGRO_ALIGN_CENTRE, "Autopista");
	}
	else if (material == 2) {
		al_draw_text(fuente, al_map_rgb(255, 255, 255), 1000, 430, ALLEGRO_ALIGN_CENTRE, "Lastre");
	}
	else if (material == 3) {
		al_draw_text(fuente, al_map_rgb(255, 255, 255), 1000, 430, ALLEGRO_ALIGN_CENTRE, "Arena");
	}
	else {
		al_draw_text(fuente, al_map_rgb(255, 255, 255), 1000, 430, ALLEGRO_ALIGN_CENTRE, "Tierra");
	}

	al_draw_text(fuente, al_map_rgb(0, 0, 255), 1000, 480, ALLEGRO_ALIGN_CENTRE, "Pendiente");
	if (material == 1) {
		al_draw_text(fuente, al_map_rgb(255, 255, 255), 1000, 510, ALLEGRO_ALIGN_CENTRE, "Plano");
	}
	else if (material == 2) {
		al_draw_text(fuente, al_map_rgb(255, 255, 255), 1000, 510, ALLEGRO_ALIGN_CENTRE, "Bajada");
	}
	else {
		al_draw_text(fuente, al_map_rgb(255, 255, 255), 1000, 510, ALLEGRO_ALIGN_CENTRE, "Subida");
	}

	al_flip_display();
}

//dibujarSalud: función encargada de dibujar en pantalla la salud actual
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void dibujarSalud() {
	char saludCadena[10];
	sprintf(saludCadena, "%.2f", personaje->salud);
	al_draw_text(fuente, al_map_rgb(255, 0, 0), 100, 25, ALLEGRO_ALIGN_CENTRE, "Salud");
	al_draw_text(fuente, al_map_rgb(255, 255, 255), 100, 55, ALLEGRO_ALIGN_CENTRE, saludCadena);
	al_flip_display();
}

//dibujarVelocidad: función encargada de dibujar en pantalla la velocidad
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void dibujarVelocidad() {
	char velCadena[10];
	sprintf(velCadena, "%.f", velocidad);
	al_draw_text(fuente, al_map_rgb(255, 0, 0), 100, 185, ALLEGRO_ALIGN_CENTRE, "Velocidad");
	al_draw_text(fuente, al_map_rgb(255, 255, 255), 100, 215, ALLEGRO_ALIGN_CENTRE, velCadena);
	al_flip_display();
}

//dibujarMarcha: función encargada de dibujar en pantalla el la velovidad
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void dibujarMarcha() {
	char marCadena[10];
	sprintf(marCadena, "%.i", marcha);
	al_draw_text(fuente, al_map_rgb(255, 0, 0), 100, 265, ALLEGRO_ALIGN_CENTRE, "Marcha");
	al_draw_text(fuente, al_map_rgb(255, 255, 255), 100, 295, ALLEGRO_ALIGN_CENTRE, marCadena);
	al_flip_display();
}

//dibujarVidas: función encargada de dibujar en pantalla las vidas actuales
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void dibujarVidas() {
	char vidasCadena[10];
	sprintf(vidasCadena, "%i", personaje->vidas);
	al_draw_text(fuente, al_map_rgb(255, 0, 0), 100, 105, ALLEGRO_ALIGN_CENTRE, "Vidas");
	al_draw_text(fuente, al_map_rgb(255, 255, 255), 100, 135, ALLEGRO_ALIGN_CENTRE, vidasCadena);
	al_flip_display();
}


//dibujarBonus: función encargada de dibujar en pantalla los bonus disponibles
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: se evalúa si hay bonus activados en el array (valor diferente de NULL)
void dibujarBonus() {
	for (int i = 0; i < numeroBonus; i++) {
		if (bonus[i] != NULL) {
			al_set_target_bitmap(al_get_backbuffer(pantalla));

			if (bonus[i]->tipo == 0) al_draw_bitmap(bonusSalud, bonus[i]->x, bonus[i]->y, NULL);

			else if (bonus[i]->tipo == 1) al_draw_bitmap(bonusVida, bonus[i]->x, bonus[i]->y, NULL);

			al_flip_display();
		}
	}

}


//dibujarPrincipal: función encargada de dibujar en pantalla al personaje principal
//Entradas: eje x, eje y, dirección del personaje
//Salidas: ninguna
//Restricciones: se toma en cuenta si es dirección izquierda o derecha, por lo que se tomará la imagen correspondiente
void dibujarPrincipal(int x, int y, int direccion) {

	switch (direccion) {
	case A: //en el caso de que la tela presionada sea la A, lo mueve a la izquierda
		if (personaje->salud > 0 && personaje->salud <= 100) { // mientras que la salus del personaje este entre cero y cien, osea, que todavia tenga salud
			al_draw_bitmap(principalIzquierda, x, y, NULL); // diuja al personaje cambiando la posicion x,y
			ultimo = principalIzquierda;
		}

		break;

	case D: // en elcaso de que la letra presionada sea la D, lo mueve a la derecha
		if (personaje->salud > 0 && personaje->salud <= 100) {
			al_draw_bitmap(principalDerecha, x, y, NULL);
			ultimo = principalDerecha;
		}

		break;

	default:
		al_draw_bitmap(ultimo, x, y, NULL);
		break;
	}

	al_flip_display();


}

//dibujar Carretera: función encargada de dibujar en pantalla la carretera
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void dibujarcarretera() {
	al_set_target_bitmap(carretera1);

	al_set_target_bitmap(al_get_backbuffer(pantalla));
	al_draw_bitmap(carretera1, 300, ncarretera1->y, NULL);
	al_flip_display();
}

//dibujar Carretera: función encargada de dibujar en pantalla la carretera
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void dibujarcarretera2() {
	al_set_target_bitmap(carretera2);

	al_set_target_bitmap(al_get_backbuffer(pantalla));
	al_draw_bitmap(carretera2, 300, ncarretera2->y, NULL);
	al_flip_display();
}

void animarCarretera(int vel) {
	if (ncarretera1->y >= largoPantalla) {
		ncarretera1->y = -600;
		ncarretera2->y = 0;
	}
	if (ncarretera2->y >= largoPantalla) {
		ncarretera2->y = -600;
		ncarretera1->y = 0;
	}
	else {
		if (velocidad <= 300 && velocidad >= 5) {
			ncarretera1->y += vel / 5;
			ncarretera2->y += vel / 5;
		}
		else if (velocidad < 5 && velocidad > 0) {
			ncarretera1->y += vel - 1;
			ncarretera2->y += vel - 1;
		}
	}
}


//dibujarEnemigoTriangulo: función encargada de dibujar en pantalla al enemigo triángulo 
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void dibujarEnemigoTriangulo() {
	for (int i = 0; i < numeroEnemigosTriangulo; i++) {

		if (enemigosTriangulo[i] != NULL) {
			al_set_target_bitmap(enemigoTrianguloBuffer);
			al_draw_bitmap_region(enemigoTriangulo, enemigosTriangulo[i]->sprite * 100, 0, 100, 100, 0, 0, NULL);
			al_set_target_bitmap(al_get_backbuffer(pantalla));
			al_draw_bitmap(enemigoTrianguloBuffer, enemigosTriangulo[i]->x, enemigosTriangulo[i]->y, NULL);
			al_flip_display();
		}
	}
}


//dibujarBalasEnemigoTriangulo: función encargada de dibujar en pantalla las balas que tira el enemigo triángulo
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: se evalúa si la bala está activada (valor diferente de NULL)
void dibujarBalasEnemigoTriangulo() {
	for (int i = 0; i < numeroBalasEnemigo; i++) {
		if (balasEnemigo[i] != NULL) {
			al_set_target_bitmap(al_get_backbuffer(pantalla));
			al_draw_bitmap(balaEnemigo, balasEnemigo[i]->x, balasEnemigo[i]->y, NULL);
			al_flip_display();
		}
	}
}


//dibujarBalasPersonaje: función encargada de dibujar en pantalla las balas que irá disparando el personaje principal
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: se evaluará si hay una estructura bala en el array disponible (que no tenga valor NULL)
void dibujarBalasPersonaje() {
	for (int i = 0; i < numeroBalasPersonaje; i++) {

		if (balasPersonaje[i] != NULL) {

			al_set_target_bitmap(al_get_backbuffer(pantalla));
			if (balasPersonaje[i]->orientacion == W || balasPersonaje[i]->orientacion == S)

				al_draw_bitmap(balaPersonajeV, balasPersonaje[i]->x, balasPersonaje[i]->y, NULL);

			if (balasPersonaje[i]->orientacion == A || balasPersonaje[i]->orientacion == D)

				al_draw_bitmap(balaPersonajeH, balasPersonaje[i]->x, balasPersonaje[i]->y, NULL);

			al_flip_display();
		}
	}
}


//dibujarMejoresPuntajes: función encargada de dibujar en pantalla el top 10 de los puntajes del juego
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void dibujarMejoresPuntajes() {
	char puntajeCadena[10];
	char posicionCadena[10];
	for (int i = 0; i < mejoresPuntajesJuego; i++) {
		sprintf(puntajeCadena, "%i", mejoresPuntajes[i]);
		sprintf(posicionCadena, "%i", i + 1);

		al_draw_text(fuente, al_map_rgb(0, 255, 0), 20, i * 30, ALLEGRO_ALIGN_CENTRE, posicionCadena);
		al_draw_text(fuente, al_map_rgb(0, 255, 0), 45, i * 30, ALLEGRO_ALIGN_CENTRE, " - ");
		al_draw_text(fuente, al_map_rgb(0, 0, 255), 105, i * 30, ALLEGRO_ALIGN_CENTRE, puntajeCadena);

	}
	al_flip_display();
}


//limpiarPantalla: función que limpia el elemento display creado al inicio del programa
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void limpiarPantalla() {
	al_clear_to_color(transparente);
}




//	_________________________________________________________________________________________		PUNTAJES





//limpiarPuntajes: función encargada de setear en 0 los elementos del array de puntajes
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void limpiarPuntajes() {
	for (int i = 0; i < mejoresPuntajesJuego; i++) {
		mejoresPuntajes[i] = 0;
	}
}


//guardarPuntajes: función encargada de guardar en un txt los puntajes obtenidos en el juego
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void guardarPuntajes() {
	documento = fopen("Puntajes.txt", "a");
	fprintf(documento, "%i \n", puntaje);
	fclose(documento);

}


//cargarPuntajes: función encargada de cargar los puntajes obtenidos en el juego
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void cargarPuntajes() {
	char puntajeTxt[10];
	int punt;
	int aux;
	documento = fopen("Puntajes.txt", "r");
	while (!feof(documento)) {
		fgets(puntajeTxt, 10, documento);
		punt = atoi(puntajeTxt);
		for (int i = 0; i < mejoresPuntajesJuego; i++) {
			if (punt >= mejoresPuntajes[i]) {
				aux = mejoresPuntajes[i];
				mejoresPuntajes[i] = punt;
				punt = aux;
			}
		}

	}
}


//sumarPuntaje: función encargada de aumentar el puntaje obtenido en el juego
//Entradas: el valor que se sumará
//Salidas: ninguna
//Restricciones: ninguna
void sumarPuntaje(int suma) {
	puntaje += suma;
}



//	_________________________________________________________________________________________		DISPAROS



//crearBalaEnemigoTriangulo: función encargada de crear una nueva bala en el array de balas del enemigo
//Entradas: eje x, eje y
//Salidas: ninguna
//Restricciones: se evalúa si hay un valor desactivado (NULL) para asignar la bala
void crearBalaEnemigoTriangulo(int x, int y) {
	for (int i = 0; i < numeroBalasEnemigo; i++) {
		if (balasEnemigo[i] == NULL) {
			balasEnemigo[i] = new BalaEnemigo(x, y, dannoBalaEnemigo);
			break;
		}
	}
}


//dispararEnemigoTriangulo: función encargada de llamar a la función para crear balas del enemigo en caso de cumplirse cierta condición
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: se evalúa que haya enemigos activados (diferente de NULL), se evalúa si se cumple el rango para lanzar la bala
void dispararEnemigoTriangulo() {
	for (int i = 0; i < numeroEnemigosTriangulo; i++) {

		if (enemigosTriangulo[i] != NULL) {

			if (abs(enemigosTriangulo[i]->x - personaje->x) == 50) {
				crearBalaEnemigoTriangulo(enemigosTriangulo[i]->x, enemigosTriangulo[i]->y);
				reproducirDisparoEnemigo();
				numenemigos++;
			}

		}
	}
}


//dispararPersonaje: función encargada de crear nuevas balas
//Entradas: la dirección (UP, DOWN, LEFT, RIGHT)
//Salidas: ninguna
//Restricciones: se evalúa si existe algún espacio disponible en el array (o sea, si es NULL)
void dispararPersonaje(int direccion) {
	if (direccion == W || direccion == D || direccion == A) {

		for (int i = 0; i < numeroBalasPersonaje; i++) {

			if (balasPersonaje[i] == NULL) {
				balasPersonaje[i] = new BalaPersonaje(personaje->x, personaje->y, direccion);
				reproducirDisparoPersonaje();
				balasdisp++;
				balasperdidas++;
				break;

			}
		}
	}
}



//	_________________________________________________________________________________________		DESACTIVACION



//desactivarBalaPersonaje: función encargada de dejar un espacio libre en el array de balas
//Entradas: posición del array que se liberará
//Salidas: ninguna
//Restricciones: ninguna
void desactivarturboPersonaje(float turb) {

	if (turb == true) {
		//cambiovelocidad();
		turb = false;
	}
}


//desactivarBonus: función encargada de desactivar el bonus del array (hacerlo NULL)
//Entradas: la posición del array que se elimina
//Salidas: ninguna
//Restricciones: ninguna
void desactivarBonus(int posicion) {
	bonus[posicion] = NULL;
}


//desactivarBalaEnemigoTriangulo: función encargada de desactivar balas del array de balas de enemigos
//Entradas: posición
//Salidas: ninguna
//Restricciones: ninguna
void desactivarBalaEnemigoTriangulo(int posicion) {

	balasEnemigo[posicion] = NULL;
}

//desactivarBalaPersonaje: función encargada de dejar un espacio libre en el array de balas
//Entradas: posición del array que se liberará
//Salidas: ninguna
//Restricciones: ninguna
void desactivarBalaPersonaje(int posicion) {

	balasPersonaje[posicion] = NULL;
}



//	_________________________________________________________________________________________		MOVIMIENTO Y VELOCIDAD




//cambiarSpriteEnemigoTriangulo: función encargada de sumar en uno la variable split del enemigo triángulo y moverlo aleatoriamente por la pantalla
/*el enemigo triangulo en realidad es invisible en el juego pero se mueve en la parte superior de la pantalla
para estar lanzando al azar sus balas, en este caso, los autos rojos que avanzan contra el personaje
*/
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: si el sumarSprite es 0, se suma en uno el sprite del enemigo triángulo
void cambiarSpriteEnemigoTriangulo() {

	for (int i = 0; i < numeroEnemigosTriangulo; i++) {

		if (enemigosTriangulo[i] != NULL) {

			if (enemigosTriangulo[i]->sprite == 1) enemigosTriangulo[i]->sumando = false;

			if (enemigosTriangulo[i]->sprite == 0) enemigosTriangulo[i]->sumando = true;

			if (enemigosTriangulo[i]->sumando) {
				if (enemigosTriangulo[i]->sumarSprite == 0) {
					enemigosTriangulo[i]->sprite += 1;
					enemigosTriangulo[i]->sumarSprite = 30;
				}

				else enemigosTriangulo[i]->sumarSprite -= 1;
			}

			if (!enemigosTriangulo[i]->sumando) {
				if (enemigosTriangulo[i]->sumarSprite == 0) {
					enemigosTriangulo[i]->sprite -= 1;
					enemigosTriangulo[i]->sumarSprite = 30;
				}

				else enemigosTriangulo[i]->sumarSprite -= 1;
			}
		}
	}
}

//moverEnemigoTriangulo: función encargada de sumar los ejer x,y del enemigo triángulo
//Entradas: movimiento (lo que se le suma a los ejes), tiempo (si es segundo o tercer timer)
//Salidas: ninguna
//Restricciones: se evalúa si el enemigo está al punto mínimo o máximo del eje x, para así cambiar el rumbo//<770 >300
void moverEnemigoTriangulo(int movimiento, int tiempo) {
	for (int i = 0; i < numeroEnemigosTriangulo; i++) {

		if (enemigosTriangulo[i]->tiempo == tiempo) {

			if (enemigosTriangulo[i]->direccion == A) {
				if (enemigosTriangulo[i]->x == 300) enemigosTriangulo[i]->direccion = D;
				else enemigosTriangulo[i]->x -= movimiento;
			}

			if (enemigosTriangulo[i]->direccion == D) {
				if (enemigosTriangulo[i]->x == (770)) enemigosTriangulo[i]->direccion = A;
				else enemigosTriangulo[i]->x += movimiento;
			}
		}
	}
}


//moverPersonaje: función encargada de sumar los ejer x,y del personaje principal
//Entradas: movimiento (lo que se le suma a los ejes)
//Salidas: ninguna
//Restricciones: ninguna
void moverPersonaje(int movimiento) {
	if (teclasDireccion[S] && (teclaPiloto[M] == false)) {
		if (personaje->y < 450) personaje->y += movimiento;
	}
	if (teclasDireccion[W] && (teclaPiloto[M] == false)) {
		if (personaje->y >  450) personaje->y -= movimiento;
	}
	if (teclasDireccion[D] && velocidad != 0) {
		if (personaje->x < (820)) personaje->x += movimiento;
	}
	if (teclasDireccion[A] && velocidad != 0) {
		if (personaje->x > 300) personaje->x -= movimiento;
	}
}


//cambiar marcha: función encargada de cambiar la marcha
//Entradas: velocidad, para determinar la marcha
//Salidas: ninguna
//Restricciones: ninguna
int cambioMarcha() {
	bool cambia = false;
	int marchaN = marcha;
	if (velocidad > 1 && velocidad < 60) {
		marcha = 1;
	}
	if (velocidad > 60 && velocidad < 120) {
		marcha = 2;
	}
	if (velocidad > 120 && velocidad < 180) {
		marcha = 3;
	}
	if (velocidad > 180 && velocidad < 240) {
		marcha = 4;
	}
	if (velocidad > 240 && velocidad < 300) {
		marcha = 5;
	}
	if (marchaN != marcha) {
		cambia = true;
		reproducirCambioMarcha();  //reproduce sonido de cambio de marcha

	}
	return marcha;
}

//cambiar velocidad: función encargada de cambiar la velocidad del personaje principal
//Entradas: movimiento (para saber si se suma o resta velocidad)
//Salidas: ninguna
//Restricciones: ninguna
void cambiovelocidad(int movimiento) {
	if (teclasDireccion[S] && velocidad > 1) { // si se presiona la letra S se le va a empezar a restar velocidad cada vez que se presione o si se mantiene presionado. La velocidad no baja de 0
		velocidad -= 2;
	}
	if (teclasDireccion[W] && velocidad < 299) {// si se presiona la letra W se le va a empezar a sumar velocidad cada vez que se presione o si se mantiene presionda
		velocidad += 2;
	}
	cambioMarcha();
}


//moverBalaPersonaje: función encargada de cambiar el valor de los ejes de las balas disponibles en el array
//Entradas: movimiento (valor que se sumará a los ejes)
//Salidas: ninguna
//Restricciones: se evalúa si hay alguna bala acticava, además de llamar a la función para desactivarla si llega a los ejes límite
void moverBalaPersonaje(int movimiento) {
	for (int i = 0; i < numeroBalasPersonaje; i++) {

		if (balasPersonaje[i] != NULL) {

			if (balasPersonaje[i]->orientacion == S) {
				if (balasPersonaje[i]->y == (largoPantalla - 30)) desactivarBalaPersonaje(i);
				else balasPersonaje[i]->y += movimiento;
			}

			else if (balasPersonaje[i]->orientacion == W) {
				if (balasPersonaje[i]->y == 60) desactivarBalaPersonaje(i);
				else balasPersonaje[i]->y -= movimiento;
			}

			else if (balasPersonaje[i]->orientacion == D) {
				if (balasPersonaje[i]->x == (anchoPantalla - 30)) desactivarBalaPersonaje(i);
				else balasPersonaje[i]->x += movimiento;
			}

			else if (balasPersonaje[i]->orientacion == A) {
				if (balasPersonaje[i]->x == 0) desactivarBalaPersonaje(i);
				else balasPersonaje[i]->x -= movimiento;
			}
		}

	}
}


//moverBalaEnemigoTriangulo: función encargada de cambiar los ejes de las balas de los enemigos en el array
//Entradas: movimiento (valor que se suma a los ejes)
//Salidas: ninguna
//Restricciones: se evalúa si hay balas activadas (diferente de NULL), se evalúa si las balas están fuera del rango
void moverBalaEnemigoTriangulo(int movimiento) {
	for (int i = 0; i < numeroBalasEnemigo; i++) {
		if (balasEnemigo[i] != NULL) {
			if (balasEnemigo[i]->y == (largoPantalla - 30)) desactivarBalaEnemigoTriangulo(i);
			else balasEnemigo[i]->y += movimiento;
		}
	}
}

//turbo: función encargada de activar el turbo
//Entradas: la dirección (UP, DOWN, LEFT, RIGHT)
//Salidas: ninguna
//Restricciones: se evalúa si una tecla fue activada
void turboPersonaje(float direccion) {
	if (direccion == true) {
		//cambiovelocidad(15);
		int a = 1;
	}
}



//	_________________________________________________________________________________________		CONTROL GAS



//restarGasMarcha: función encargada de calcular el comsumo del auto basándose en la marcha en la que esté
//Entradas: la cantidad que va a restar por ejecución
//Salidas: ninguna
//Restricciones: la entrada debe ser un número entre 1 y 5
void restarGasMarcha(int resta) {
	int marcha = cambioMarcha();
	if (marcha == 1 && velocidad != 0) gas -= resta;
	if (marcha == 2) gas -= resta * 0.8;
	if (marcha == 3) gas -= resta * 0.6;
	if (marcha == 4) gas -= resta * 0.4;
	if (marcha == 5) gas -= resta * 0.2;
}


//consumoGasClima: función encargada de calcular el comsumo del auto basándose en el clima en el que se encuentre en ese momento
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: niguna
void consumoGasClima() {
	if (velocidad != 0) {
		if (clima == 2) gas -= 1;
		if (clima == 3) gas -= 2;
	}
}


//consumoGasMaterial: función encargada de calcular el comsumo del auto basándose en el material de la carretera en la que se encuentre en ese momento
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: niguna
void consumoGasMaterial() {
	if (velocidad != 0) {
		if (material == 2) gas -= 1.2;
		if (material == 3) gas -= 2;
		if (material == 4) gas -= 1.5;
	}
}


//consumoGasPendiente: función encargada de calcular el comsumo del auto basándose en la pendiente en la que se encuentre en ese momento
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: niguna
void consumoGasPendiente() {
	if (velocidad != 0) {
		if (pendiente == 3) gas -= 2;
	}
}



// _________________________________________________________________________________________		SALUD Y VIDA




//sumarSalud: función encargada de sumar valor a la salud del personaje
//Entradas: el valor que se le suma a la salud
//Salidas: ninguna
//Restricciones: si la suma es mayor a 100.0, se le asigna 100.0 a la salud
void sumarSalud(float salud) {
	personaje->salud += salud;
	if (personaje->salud > 100.0) personaje->salud = 100.0;
}


//sumarVida: función encargada de sumar un valor a las vidas del personaje
//Entradas: vida que se suma
//Salidas: ninguna
//Restricciones: ninguna
void sumarVida(int vida) {
	personaje->vidas += vida;
}


//restarSalud: función encargada de restar salud al personaje principal
//Entradas: el daño que se restará a la salud
//Salidas: ninguna
//Restricciones: ninguna
void restarSalud(float danno) {
	personaje->salud -= danno;
}


//restarVidas: función encargadas de restar vidas al personaje principal
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void restarVidas() {
	personaje->vidas -= 1;
}


//restaurarSalud: función encargada de restaurar toda la salud del personaje principal
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void restaurarSalud() {
	personaje->salud = 100.0;
}


//sinSalud: función encargada de evaluar si el personaje principal tiene o no salud
//Entradas: ninguna
//Salidas: true, si no tiene salud false: si aún tiene salud
//Restricciones: se evalúa si la salud es menor o igual a 0
bool sinSalud() {
	if (personaje->salud <= 0.0) return true;

	else return false;
}


//sinVidas: función encargada de evaluar si el personaje principal tiene vidas o no
//Entradas: ninguna
//Salidas: true: si aún posee vidas false: si no posee vidas
//Restricciones: se evalúa si la cantidad de vidas es igual a 0
bool sinVidas() {
	if (personaje->vidas == 0) return true;
	else return false;
}





//generarBonus: función encargada de crear un bonus en el juego (se agrega al array de bonus)
//este genera un bonus en una posicion de pantalla ya determinada
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: se evalúa si se posee espacio en el array (valor NULL)
void generarBonus() {
	int x;
	int y;

	for (int i = 0; i < numeroBonus; i++) {
		if (bonus[i] == NULL) {
			x = rand() % 500 + 300;
			y = 450;
			bonus[i] = new Bonus(x, y, i % 2, saludBonus);
			bonusgenerados++;
			bonusperdidos++;
			break;
		}
	}
}



//	_________________________________________________________________________________________		COLISIONES



//colisionBalaEnemigoTriangulo: función encargada de evaluar si existe colisión entre alguna bala del enemigo y el personaje principal
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: se evalúa si hay balas activadas (diferente de NULL)
void colisionBalaEnemigoTriangulo() {
	for (int i = 0; i < numeroBalasEnemigo; i++) {

		if (balasEnemigo[i] != NULL) {
			if ((personaje->x - 15 < balasEnemigo[i]->x && balasEnemigo[i]->x < personaje->x + 15) && (personaje->y - 15 < balasEnemigo[i]->y && balasEnemigo[i]->y < personaje->y + 15)) {
				restarSalud(balasEnemigo[i]->danno);
				desactivarBalaEnemigoTriangulo(i);
			}
		}
	}
}

//colisionBalaconBala: función encargada de evaluar si existe colisión entre alguna bala del enemigo y la bala del personaje principal
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: se evalúa si hay balas activadas (diferente de NULL)
void colisionBalaconBala() {
	for (int i = 0; i < numeroBalasEnemigo; i++) {

		if (balasEnemigo[i] != NULL && balasPersonaje[i] != NULL) {
			if ((balasPersonaje[i]->x - 50 < balasEnemigo[i]->x && balasEnemigo[i]->x < balasPersonaje[i]->x + 50) && (balasPersonaje[i]->y - 50 < balasEnemigo[i]->y && balasEnemigo[i]->y < balasPersonaje[i]->y + 50)) {
				desactivarBalaEnemigoTriangulo(i);
				enemigosel++;
				balasperdidas--;
				sumarPuntaje(10);
			}
		}

	}
}



//colisionBonus: función encargada de evaluar si existe colisión entre un bonus y el personaje principal
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: se evalúa si hay bonus activados (diferente de NULL)
void colisionBonus() {
	for (int i = 0; i < numeroBonus; i++) {
		if (bonus[i] != NULL) {
			if ((personaje->x - 30 < bonus[i]->x  &&  bonus[i]->x < personaje->x + 30) && (personaje->y - 30 < bonus[i]->y && bonus[i]->y < personaje->y + 30)) {

				if (bonus[i]->tipo == 0) {
					sumarSalud(bonus[i]->salud);
					sumarPuntaje(sumaPuntajeBonusSalud);
					bonusop++;
					bonusperdidos--;
				}

				else if (bonus[i]->tipo == 1) {
					sumarVida(1);
					sumarPuntaje(sumaPuntajeBonusVida);
					bonusop++;
					bonusperdidos--;
				}

				desactivarBonus(i);

			}
		}
	}
}



// _________________________________________________________________________________________  INICIALIZACIÓN




//iniciarPuntajes: función encargada de inicializar el array de puntajes en 0
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void iniciarPuntajes() {
	for (int i = 0; i < mejoresPuntajesJuego; i++) {
		mejoresPuntajes[i] = 0;
	}
}


//iniciarPersonaje: función encargada de iniciar el personaje principal del juego
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void iniciarPersonaje() {
	personaje = new PersonajePrincipal(anchoPantalla / 2, 450, 3, 100.0);
}


//iniciarEnemigosTriangulo: función encargada de iniciar los enemigos triángulo del juego
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void iniciarEnemigosTriangulo() {
	int x;

	for (int i = 0; i < numeroEnemigosTriangulo; i++) {
		x = rand() % 31 + 5;
		enemigosTriangulo[i] = new EnemigoTriangulo(300, 0, (i % 2 + 2), i + 1, 0, dannoEnemigoTriangulo);
	}

}


//iniciarBalasPersonaje: función encargada de iniciar las balas del personaje principal del juego
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void iniciarBalasPersonaje() {
	for (int i = 0; i < numeroBalasPersonaje; i++) {
		balasPersonaje[i] = NULL;
	}
}


//iniciarBalasEnemigo: función encargada de iniciar las balas del enemigo del juego
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void iniciarBalasEnemigo() {
	for (int i = 0; i < numeroBalasEnemigo; i++) {
		balasEnemigo[i] = NULL;
	}
}

//iniciarCarretera: función encargada de iniciarCarretera
//Entradas: Carretera *ncarretera, eje y
//Salidas: ninguna
//Restricciones: 
void iniciarCarreteras() {
	ncarretera1 = new Carretera(0);
	ncarretera2 = new Carretera(-600);
}


//iniciarBonus: función encargada de iniciar los bonus del juego
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void iniciarBonus() {
	for (int i = 0; i < numeroBonus; i++) {
		bonus[i] = NULL;
	}
}



//	_________________________________________________________________________________________	CONDICIONES de CARRETERA



//DeterminarPendiente: función encargada de determinar aleatoriamente una pendeinte
//Entradas: ninguna
//Salidas: la pendiente, ya sea de subida, plano o de bajada
//Restricciones: ninguna
void determinarPendiente() {
	pendiente = rand() % 3 + 1;
	cout << pendiente << endl;
}

//DeterminarClima: función encargada de determinar aleatoriamente un clima
//Entradas: ninguna
//Salidas: clima tipo lluvia, nevada o sol
//Restricciones: ninguna
void determinarClima() {
	clima = rand() % 3 + 1;
	cout << clima << endl;
}

//DeterminarCarretera: función encargada de determinar aleatoriamente un material para la carretera sobre la que se conduce
//Entradas: ninguna
//Salidas: el material, que puede ser arena, tierra, lastre o asfalto
//Restricciones: ninguna
void determinarMaterial() {
	material = rand() % 4 + 1;
	cout << material << endl;
}



//	_________________________________________________________________________________________	 SENSORES para el PILOTO AUTOMÁTICO

//sensorPosicion: función que compara la posición del auto con los enemigos y lo mueve en caso de que sea igual
//Entradas: ninguna
//Salidas: verdadero si el personaje y un enemigo están en la misma posición
//Restricciones: ninguna
bool sensorPosicion() {
	for (int i = 0; i < numeroBalasEnemigo; i++) {

		if (balasEnemigo[i] != NULL) {
			if ((personaje->x == balasEnemigo[i]->x) && (balasEnemigo[i]->y >= 200)) {
				return true;
			}
			else return false;
		}
		else return false;
	}
}


//moverAutomatico: función encargada de mover el auto en base a sensor posición, evitando colisiones
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: niguna
void moverAutomatico() {
	if (sensorPosicion()) {
		if (personaje->x < anchoPantalla / 2) {
			personaje->x += 25;
		}
		else {
			personaje->x -= 25;
		}
	}
}


//cambiarVelocidadPendiente: función encargada de cambiar la velocidad del auto en base a la pendiente
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: niguna
void cambiarVelocidadPendiente() {//siempre que pendiente sea 3, 45 velocidad
	if (pendiente == 3) {
		if (velocidad != 45) {
			if (velocidad > 45) velocidad -= 2;
			else velocidad += 2;
		}
	}
}


//cambiarVelocidadClima: función encargada de cambiar la velocidad del auto en base al clima
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: niguna
void cambiarVelocidadClima() {
	if (pendiente != 3) {
		if (clima == 2) {
			if (velocidad != 50) {
				if (velocidad > 50) {
					velocidad -= 2;
				}
				else velocidad += 2;
			}
		}
		if (clima == 3) {
			if (velocidad != 30) {
				if (velocidad > 30) {
					velocidad -= 2;
				}
				else velocidad += 2;
			}
		}
	}
}

//cambiarVelocidadMaterial: función encargada de cambiar la velocidad del auto en base al material de la carretera
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: niguna
void cambiarVelocidadMaterial() {
	if (clima == 1 && pendiente != 3) {
		if (material == 2) {
			if (velocidad != 60) {
				if (velocidad > 60) velocidad -= 2;
				else velocidad += 2;
			}
		}
		if (material == 3) {
			if (velocidad != 20) {
				if (velocidad > 20) velocidad -= 2;
				else velocidad += 2;
			}
		}
		if (material == 4) {
			if (velocidad != 40) {
				if (velocidad > 40) velocidad -= 2;
				else velocidad += 2;
			}
		}
		if (material == 1) {
			if (velocidad < 300) velocidad += 2;
		}
	}
}



//		M   M	   A	  I I I	   N     N
//		M M M	  A A	    I	   N N   N
//		M	M	 A A A	    I	   N   N N
//		M   M   A     A   I I I	   N     N




int main(int argc, char **argv) {



	if (!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}

	//Esta línea de código permite que la ventana tenga la capacidad de cambiar de tamaño
	al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE);

	//crea la pantalla del juego
	pantalla = al_create_display(anchoPantalla, largoPantalla);
	al_set_window_position(pantalla, 100, 50);
	al_set_window_title(pantalla, "Auto Fantásticos");
	if (!pantalla) {
		fprintf(stderr, "failed to create display!\n");
		return -1;
	}

	transparente = al_map_rgb(0, 0, 0);

	//Líneas para obtener las funcionalidades del uso de las fuentes
	//*******************
	al_init_font_addon();
	al_init_ttf_addon();
	//*******************

	fuente = al_load_font("Over There.ttf", 15, NULL);


	al_init_primitives_addon();

	//Línea para obtener las funcionalidades de las imágenes
	//*******************
	al_init_image_addon();
	//*******************

	//Se crean los contenedores de las imágenes que se utilizarán en el juego
	//*******************
	principalBuffer = al_create_bitmap(30, 30);
	enemigoTrianguloBuffer = al_create_bitmap(60, 60);
	carretera1 = al_create_bitmap(20, 30);
	carretera2 = al_create_bitmap(20, 30);
	//*******************

	//Se cargan las imágenes que se van a utilizar en el juego
	//*******************
	principalIzquierda = al_load_bitmap("Imagenes/car.png");
	principalDerecha = al_load_bitmap("Imagenes/car.png");
	enemigoTriangulo = al_load_bitmap("Imagenes/invisible.png");
	balaPersonajeH = al_load_bitmap("Imagenes/bala3.png");
	balaPersonajeV = al_load_bitmap("Imagenes/bala3.png");
	balaEnemigo = al_load_bitmap("Imagenes/carop.png");
	bonusSalud = al_load_bitmap("Imagenes/estrella.png");
	bonusVida = al_load_bitmap("Imagenes/estrella.png");
	carretera1 = al_load_bitmap("Imagenes/background1.png");
	carretera2 = al_load_bitmap("Imagenes/background1.png");
	//*******************

	//Líneas para obtener las funcionalidades de los audios
	//*******************
	al_install_audio();
	al_init_acodec_addon();
	al_reserve_samples(1000);
	//*******************

	//Se cargan los audios que se van a utilizar en el juego
	//*******************
	musicaJuego = al_load_sample("Musica/musicaautofantantico3.wav");
	turbPersonaje = al_load_sample("Musica/bala.ogg");
	disparoPersonaje = al_load_sample("Musica/bala.ogg");
	marchaCambia = al_load_sample("Musica/cambiaMarcha.ogg");
	voz_bajevel = al_load_sample("Musica/bajevelocidad.ogg");
	voz_gasbajo = al_load_sample("Musica/nivelgasbajo.ogg");
	voz_saludbaja = al_load_sample("Musica/saludbaja.ogg");
	disparoEnemigo = al_load_sample("Musica/Laser2.WAV");
	colisionEnemigo = al_load_sample("Musica/ow.WAV");
	//*******************

	//Líneas para obtener las funcionalidades del teclado
	//*******************
	al_init_primitives_addon();
	al_install_keyboard();
	//*******************



	bool hecho, menu = true;
	int disparo = -1;
	int movimiento = 10;
	int direccion = D;


	//Líneas de código para la llamada del menu principal

	//*******************************************************************************
	dibujarMenu();
	reproducirMusicaJuego();
	ALLEGRO_KEYBOARD_STATE estadoTeclado;

	al_get_keyboard_state(&estadoTeclado);

	ALLEGRO_EVENT eventoMenu;

	ALLEGRO_EVENT_QUEUE *colaEventoMenu = al_create_event_queue();

	al_register_event_source(colaEventoMenu, al_get_keyboard_event_source());

	iniciarPuntajes();

	// menu inicial
	while (menu) {
		al_wait_for_event(colaEventoMenu, &eventoMenu);

		if (eventoMenu.type = ALLEGRO_EVENT_KEY_DOWN) {

			if (eventoMenu.keyboard.keycode == ALLEGRO_KEY_1) {
				hecho = true;
				menu = false;
			}

			else if (eventoMenu.keyboard.keycode == ALLEGRO_KEY_2) {
				limpiarPantalla();
				cargarPuntajes();
				dibujarMejoresPuntajes();
				limpiarPuntajes();
			}

			else if (eventoMenu.keyboard.keycode == ALLEGRO_KEY_3) {
				hecho = false;
				menu = false;
			}

			else if (eventoMenu.keyboard.keycode == ALLEGRO_KEY_S) {
				limpiarPantalla();
				dibujarMenu();
			}
		}
	}
	//*******************************************************************************

	//Timers que se necesitarán para el juego
	//**********************************************************
	ALLEGRO_TIMER *primerTimer = al_create_timer(1.0 / FPS);
	ALLEGRO_TIMER *segundoTimer = al_create_timer(1.0 / FPS1);
	ALLEGRO_TIMER *tercerTimer = al_create_timer(1.0 / FPS2);
	ALLEGRO_TIMER *cuartoTimer = al_create_timer(1.0 / FPS3);
	ALLEGRO_TIMER *quintoTimer = al_create_timer(5.0 / FPS4);
	ALLEGRO_TIMER *sextoTimer = al_create_timer(20.0 / FPS4);
	ALLEGRO_TIMER *septimoTimer = al_create_timer(1.0 / FPS5);
	ALLEGRO_TIMER *octavoTimer = al_create_timer(1.0 / FPS6);
	ALLEGRO_TIMER *novenoTimer = al_create_timer(1.0 / FPS);
	ALLEGRO_TIMER *decimoTimer = al_create_timer(1.0 / FPS8);
	ALLEGRO_TIMER *undecimoTimer = al_create_timer(1.0 / FPS9);
	ALLEGRO_TIMER *duodecimoTimer = al_create_timer(1.0 / FPS);
	ALLEGRO_TIMER *treceavoTimer = al_create_timer(1.0 / FPS10);
	//**********************************************************

	//Se crea una cola de eventos
	ALLEGRO_EVENT_QUEUE *colaEventos = al_create_event_queue();

	//Registro de los eventos
	//**********************************************************
	al_register_event_source(colaEventos, al_get_timer_event_source(primerTimer));
	al_register_event_source(colaEventos, al_get_timer_event_source(segundoTimer));
	al_register_event_source(colaEventos, al_get_timer_event_source(tercerTimer));
	al_register_event_source(colaEventos, al_get_timer_event_source(cuartoTimer));
	al_register_event_source(colaEventos, al_get_timer_event_source(quintoTimer));
	al_register_event_source(colaEventos, al_get_timer_event_source(sextoTimer));
	al_register_event_source(colaEventos, al_get_timer_event_source(septimoTimer));
	al_register_event_source(colaEventos, al_get_timer_event_source(octavoTimer));
	al_register_event_source(colaEventos, al_get_timer_event_source(novenoTimer));
	al_register_event_source(colaEventos, al_get_timer_event_source(decimoTimer));
	al_register_event_source(colaEventos, al_get_timer_event_source(undecimoTimer));
	al_register_event_source(colaEventos, al_get_timer_event_source(duodecimoTimer));
	al_register_event_source(colaEventos, al_get_timer_event_source(treceavoTimer));
	al_register_event_source(colaEventos, al_get_keyboard_event_source());
	//**********************************************************


	//Inicialización de los timer
	//**********************************************************
	al_start_timer(primerTimer);
	al_start_timer(segundoTimer);
	al_start_timer(tercerTimer);
	al_start_timer(cuartoTimer);
	al_start_timer(quintoTimer);
	al_start_timer(sextoTimer);
	al_start_timer(septimoTimer);
	al_start_timer(octavoTimer);
	al_start_timer(novenoTimer);
	al_start_timer(decimoTimer);
	al_start_timer(undecimoTimer);
	al_start_timer(duodecimoTimer);
	al_start_timer(treceavoTimer);

	//**********************************************************

	//Llamado a las funciones que inicializan los componentes lógicos del juego
	//**********************************************************
	iniciarPersonaje();
	iniciarEnemigosTriangulo();
	iniciarBalasPersonaje();
	iniciarBalasEnemigo();
	iniciarBonus();

	iniciarCarreteras();
	//**********************************************************


	while (hecho) {

		ALLEGRO_EVENT eventos;

		al_wait_for_event(colaEventos, &eventos);


		/*Evento que toma en cuenta la tecla más actual presionada, se van activando en un array que representa cada movimiento
		Observación: Si se presiona una segunda tecla, al mismo tiempo que se está presionando la primera, este evento será capaz de detectar las dos teclas
		y representarlas en el array de activación (teclas)
		*/
		if (eventos.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch (eventos.keyboard.keycode) {

			case ALLEGRO_KEY_S:
				teclasDireccion[S] = true;
				direccion = S;
				break;

			case ALLEGRO_KEY_W:
				teclasDireccion[W] = true;
				direccion = W;
				break;

			case ALLEGRO_KEY_D:
				teclasDireccion[D] = true;
				direccion = D;
				break;

			case ALLEGRO_KEY_A:
				teclasDireccion[A] = true;
				direccion = A;
				break;

			case ALLEGRO_KEY_J:
				disparo = A;
				break;

			case ALLEGRO_KEY_I:
				disparo = W;
				break;

			case ALLEGRO_KEY_L:
				disparo = D;
				break;

			case ALLEGRO_KEY_M:
				if (teclaPiloto[M] == true) teclaPiloto[M] = false;
				else teclaPiloto[M] = true;
				break;

			case ALLEGRO_KEY_ESCAPE:
				guardarPuntajes();
				hecho = false;
				break;
			}

		}
		/*Evento que toma en cuenta cuando se deja de presionar alguna tecla, funciona de la misma forma que el evento anterior, cuando se deja de presionar una
		tecla, se desactiva del array de activación (teclas)
		*/
		if (eventos.type == ALLEGRO_EVENT_KEY_UP) {
			switch (eventos.keyboard.keycode) {

			case ALLEGRO_KEY_S:
				teclasDireccion[S] = false;
				break;

			case ALLEGRO_KEY_W:
				teclasDireccion[W] = false;
				break;

			case ALLEGRO_KEY_D:
				teclasDireccion[D] = false;
				break;

			case ALLEGRO_KEY_A:
				teclasDireccion[A] = false;
				break;


			case ALLEGRO_KEY_ESCAPE:
				hecho = true;
				break;
			}

		}


		//condicional que analiza los timers y llama a distintas funciones

		if (eventos.type == ALLEGRO_EVENT_TIMER) {
			if (eventos.timer.source == primerTimer) {
				moverPersonaje(movimiento);
			}

			else if (eventos.timer.source == segundoTimer) {
				moverEnemigoTriangulo(movimiento, 1);
			}

			else if (eventos.timer.source == tercerTimer) {
				moverEnemigoTriangulo(movimiento, 2);

			}

			else if (eventos.timer.source == cuartoTimer) {
				for (int s = 0; s < 2; s++) {
					turboPersonaje(turb);
				}
				dispararPersonaje(disparo);
				disparo = -1;
				moverBalaPersonaje(movimiento);
				dispararEnemigoTriangulo();
				moverBalaEnemigoTriangulo(movimiento);
				if (turb == true) {
					for (int j = 0; j < 1; j++) {
						desactivarturboPersonaje(turb);
					}
				}
			}

			else if (eventos.timer.source == quintoTimer) {
				sumarPuntaje(sumaPuntajeTimer);
				restarGasMarcha(restagastimer);
				consumoGasClima();
				consumoGasMaterial();
				consumoGasPendiente();
			}

			else if (eventos.timer.source == sextoTimer) {
				generarBonus();
			}

			else if (eventos.timer.source == septimoTimer) {
				dibujarcarretera();
				dibujarcarretera2();
				dibujarPrincipal(personaje->x, personaje->y, direccion);
				dibujarEnemigoTriangulo();
				dibujarBalasPersonaje();
				dibujarBalasEnemigoTriangulo();
				dibujarBonus();
				dibujarPuntaje();
				dibujargas();
				dibujarMarcha();
				dibujarSalud();
				dibujarVidas();
				dibujarVelocidad();
				dibujarcontroles();
				dibujarCondiciones();

			}

			else if (eventos.timer.source == octavoTimer) {
				moverEnemigoTriangulo(movimiento, 3);
			}

			else if (eventos.timer.source == novenoTimer && (teclaPiloto[M] == true)) {
				sensorPosicion();
				moverAutomatico();

			}

			else if (eventos.timer.source == decimoTimer) {
				determinarMaterial();
				determinarClima();
				determinarPendiente();
			}

			else if (eventos.timer.source == undecimoTimer) {
				cambiovelocidad(movimiento);
				animarCarretera(velocidad);
			}

			else if (eventos.timer.source == duodecimoTimer && (teclaPiloto[M] == true)) {
				cambiarVelocidadPendiente();
				cambiarVelocidadClima();
				cambiarVelocidadMaterial();
			}
			else if (eventos.timer.source == treceavoTimer) {
				reproducirBajeVel();
				reproducirGasBajo();
				reproducirSaludBaja();
			}
		}

		cambiarSpriteEnemigoTriangulo();
		limpiarPantalla();
		colisionBalaconBala();
		colisionBalaEnemigoTriangulo();
		colisionBalaconBala();
		colisionBonus();

		if (sinSalud()) {
			restarVidas();
			restaurarSalud();
		}

		if (sinVidas()) {
			hecho = false;
			guardarPuntajes();
		}

		if (gas <= 0) {
			hecho = false;
			guardarPuntajes();
		}

	}



	//Se liberan los elementos de allegro que se utilizaron

	al_destroy_font(fuente);

	al_destroy_display(pantalla);

	al_destroy_event_queue(colaEventos);
	al_destroy_event_queue(colaEventoMenu);



	al_destroy_bitmap(principalIzquierda);
	al_destroy_bitmap(principalDerecha);
	al_destroy_bitmap(principalAturdidoIzquierda);
	al_destroy_bitmap(principalAturdidoDerecha);
	al_destroy_bitmap(principalCriticoIzquierda);
	al_destroy_bitmap(principalCriticoDerecha);
	al_destroy_bitmap(enemigoTriangulo);
	al_destroy_bitmap(balaPersonajeH);
	al_destroy_bitmap(balaPersonajeV);
	al_destroy_bitmap(balaEnemigo);
	al_destroy_bitmap(enemigoTrianguloBuffer);
	al_destroy_bitmap(bonusSalud);
	al_destroy_bitmap(bonusVida);
	al_destroy_bitmap(carretera1);
	al_destroy_bitmap(carretera2);

	al_destroy_sample(musicaJuego);
	al_destroy_sample(turbPersonaje);
	al_destroy_sample(disparoEnemigo);
	al_destroy_sample(marchaCambia);
	al_destroy_sample(voz_bajevel);
	al_destroy_sample(voz_gasbajo);
	al_destroy_sample(voz_saludbaja);
	al_destroy_sample(colisionEnemigo);

	al_destroy_sample_instance(instanciaSonido);

	al_destroy_timer(primerTimer);
	al_destroy_timer(segundoTimer);
	al_destroy_timer(tercerTimer);
	al_destroy_timer(cuartoTimer);
	al_destroy_timer(quintoTimer);
	al_destroy_timer(sextoTimer);
	al_destroy_timer(septimoTimer);
	al_destroy_timer(octavoTimer);
	al_destroy_timer(novenoTimer);
	al_destroy_timer(decimoTimer);
	al_destroy_timer(undecimoTimer);
	al_destroy_timer(duodecimoTimer);
	al_destroy_timer(treceavoTimer);

	return 0;
}
