//####################                 Auto Fantastico              ####################

// Mejorado por Alejandro Hernández y Luciano Cendra

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
enum Teclas { J, I, K, L };

//Se definen como false al inicio las teclas, osea, que no han sido presionadas
bool teclasDireccion[4] = { false, false, false, false };
bool teclasDisparo[4] = { false, false, false, false };

//Definicion de variables globales necesarias en el juego
int puntaje = 0;
int gas = 100;
int gasusado = 0;
float velocidad = 90.0;
bool turb = false;
int balasdisp = 0;
int bonusop = 0;
int enemigosel = 0;
int balasperdidas = 0;
int numenemigos = 0;
int bonusgenerados = 0;
int bonusperdidos = 0;

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

ALLEGRO_BITMAP *principalBuffer;
ALLEGRO_BITMAP *enemigoTrianguloBuffer;
ALLEGRO_BITMAP *carretera;

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
ALLEGRO_SAMPLE *turbPersonaje;
ALLEGRO_SAMPLE *disparoEnemigo;
ALLEGRO_SAMPLE *colisionEnemigo;

ALLEGRO_SAMPLE_INSTANCE *instanciaSonido;

//ALLEGRO_FONT *fuente;
ALLEGRO_FONT *fuente = al_load_font("JacobiStriped.ttf", 16, NULL);

//**********************************************************

//FUNCIONES NECESARIAS EN EL JUEGO

//reproducirMusicaJuego: función encargada de reproducir el tema principal del juego
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void reproducirMusicaJuego(){
	al_play_sample(musicaJuego, 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
}


//reproducirTurboPersonaje: función encargada de reproducir el sonido para el turbo del personaje
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void reproducirturboPersonaje(){
	instanciaSonido = al_create_sample_instance(turbPersonaje);
	al_set_sample_instance_playmode(instanciaSonido, ALLEGRO_PLAYMODE_ONCE);
	al_attach_sample_instance_to_mixer(instanciaSonido, al_get_default_mixer());
	al_play_sample_instance(instanciaSonido);
}


//reproducirDisparoEnemigo: función encargada de reproducir el sonido para el disparo del enemigo
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void reproducirDisparoEnemigo(){
	instanciaSonido = al_create_sample_instance(disparoEnemigo);
	al_set_sample_instance_playmode(instanciaSonido, ALLEGRO_PLAYMODE_ONCE);
	al_attach_sample_instance_to_mixer(instanciaSonido, al_get_default_mixer());
	al_play_sample_instance(instanciaSonido);
}


//reproducirColisionEnemigo: función encargada de reproducir el sonido para la colisión entre el enemigo y la bala del personaje
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void reproducirColisionEnemigo(){
	instanciaSonido = al_create_sample_instance(colisionEnemigo);
	al_set_sample_instance_playmode(instanciaSonido, ALLEGRO_PLAYMODE_ONCE);
	al_attach_sample_instance_to_mixer(instanciaSonido, al_get_default_mixer());
	al_play_sample_instance(instanciaSonido);
}


//dibujarMenu: función encargada de dibujar en pantalla las opciones disponibles del menu del juego
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void dibujarMenu(){
	al_draw_text(fuente, al_map_rgb(255, 255, 255), 600, 150, ALLEGRO_ALIGN_CENTRE, "Selccione uno de los siguientes:");
	al_draw_text(fuente, al_map_rgb(255, 255, 255), 600, 260, ALLEGRO_ALIGN_CENTRE, "1- JUGAR");
	al_draw_text(fuente, al_map_rgb(255, 255, 255), 600, 320, ALLEGRO_ALIGN_CENTRE, "2- VER TABLA DE PUNTAJES");
	al_draw_text(fuente, al_map_rgb(255, 255, 255), 600, 380, ALLEGRO_ALIGN_CENTRE, "3- SALIR");
	al_flip_display();
}


//dibujarPuntaje: función encargada de dibujar en pantalla el puntaje actual
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void dibujarPuntaje(){
	char puntajeCadena[10];
	sprintf(puntajeCadena, "%i""%", puntaje);
	al_draw_text(fuente, al_map_rgb(0, 0, 255), 900, 25, ALLEGRO_ALIGN_LEFT, "Puntaje:");
	al_draw_text(fuente, al_map_rgb(0, 255, 0), 1000, 25, ALLEGRO_ALIGN_LEFT, puntajeCadena);
	al_flip_display();
}

//dibujargas: función encargada de dibujar en pantalla la gasolina restante
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void dibujargas(){
	char gasCadena[10];
	sprintf(gasCadena, "%i""%", gas);
	al_draw_text(fuente, al_map_rgb(0, 0, 255), 75, 265, ALLEGRO_ALIGN_LEFT, "Gas:");
	al_draw_text(fuente, al_map_rgb(0, 255, 0), 75, 295, ALLEGRO_ALIGN_LEFT, gasCadena);
	al_flip_display();
}

//dibujargasusado: función encargada de dibujar en pantalla la gasolina gastada
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void dibujargasusado(){
	char gasusadoCadena[10];
	sprintf(gasusadoCadena, "%i""%", gasusado);
	al_draw_text(fuente, al_map_rgb(0, 0, 255), 0, 25, ALLEGRO_ALIGN_LEFT, "Gas Utilizado:");
	al_draw_text(fuente, al_map_rgb(0, 255, 0), 190, 25, ALLEGRO_ALIGN_LEFT, gasusadoCadena);
	al_flip_display();
}


//dibujar balas disparadas: función encargada de dibujar en pantalla las balas Disparadas
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void dibujarbalasdisp(){
	char balasgastCadena[10];
	sprintf(balasgastCadena, "%i""%", balasdisp);
	al_draw_text(fuente, al_map_rgb(0, 0, 255), 0, 100, ALLEGRO_ALIGN_LEFT, "Balas Disparadas:");
	al_draw_text(fuente, al_map_rgb(0, 255, 0), 250, 100, ALLEGRO_ALIGN_LEFT, balasgastCadena);
	al_flip_display();
}

//dibujar el numero de enemigos eliminados: función encargada de dibujar en pantalla el numero de enemigos que ha sido eliminado
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void dibujarenemigosel(){
	char enemigoselCadena[10];
	sprintf(enemigoselCadena, "%i""%", enemigosel);
	al_draw_text(fuente, al_map_rgb(0, 0, 255), 0, 200, ALLEGRO_ALIGN_LEFT, "Enemigos eliminados:");
	al_draw_text(fuente, al_map_rgb(0, 255, 0), 270, 200, ALLEGRO_ALIGN_LEFT, enemigoselCadena);
	al_flip_display();
}

//dibujar numero de bonus optenidos: función encargada de dibujar en pantalla los bonus optenidos
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void dibujarnumbonus(){
	char bonusCadena[10];
	sprintf(bonusCadena, "%i""%", bonusop);
	al_draw_text(fuente, al_map_rgb(0, 0, 255), 0, 150, ALLEGRO_ALIGN_LEFT, "Bonus optenidos:");
	al_draw_text(fuente, al_map_rgb(0, 255, 0), 250, 150, ALLEGRO_ALIGN_LEFT, bonusCadena);
	al_flip_display();
}

//dibujar balas perdidas: función encargada de dibujar en pantalla el numero de balas que no chocaron contra un enemigo
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void dibujarbalasperdidas(){
	char balCadena[10];
	sprintf(balCadena, "%i""%", balasperdidas);
	al_draw_text(fuente, al_map_rgb(0, 0, 255), 0, 300, ALLEGRO_ALIGN_LEFT, "Balas perdidas:");
	al_draw_text(fuente, al_map_rgb(0, 255, 0), 250, 300, ALLEGRO_ALIGN_LEFT, balCadena);
	al_flip_display();
}

//dibujar bonus generados: función encargada de dibujar en pantalla el numero de bonus generados
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void dibujarbonusgenerados(){
	char bonCadena[10];
	sprintf(bonCadena, "%i""%", bonusgenerados);
	al_draw_text(fuente, al_map_rgb(0, 0, 255), 0, 350, ALLEGRO_ALIGN_LEFT, "Bonus generados:");
	al_draw_text(fuente, al_map_rgb(0, 255, 0), 250, 350, ALLEGRO_ALIGN_LEFT, bonCadena);
	al_flip_display();
}

//dibujar bonus perdidos: función encargada de dibujar en pantalla el numero de bonus perdidos
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void dibujarbonusperdidos(){
	char bonperCadena[10];
	sprintf(bonperCadena, "%i""%", bonusperdidos);
	al_draw_text(fuente, al_map_rgb(0, 0, 255), 0, 400, ALLEGRO_ALIGN_LEFT, "Bonus perdidos:");
	al_draw_text(fuente, al_map_rgb(0, 255, 0), 250, 400, ALLEGRO_ALIGN_LEFT, bonperCadena);
	al_flip_display();
}



//dibujar numero enemigos: función encargada de dibujar en pantalla el numero de enemigos presentados durante el juego
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void dibujarnumenemigos(){
char enCadena[10];
sprintf(enCadena, "%i""%", numenemigos);
al_draw_text(fuente, al_map_rgb(0, 0, 255), 0, 250, ALLEGRO_ALIGN_LEFT, "Numero enemigos:");
al_draw_text(fuente, al_map_rgb(0, 255, 0), 250, 250, ALLEGRO_ALIGN_LEFT, enCadena);
al_flip_display();
}


//dibujar controles del juego: función encargada de dibujar en pantalla los controles del juego
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void dibujarcontroles(){
	al_draw_text(fuente, al_map_rgb(0, 0, 255), 945, 100, ALLEGRO_ALIGN_LEFT, "Manejo:");
	al_draw_text(fuente, al_map_rgb(0, 255, 0), 955, 130, ALLEGRO_ALIGN_LEFT, "A,W,S,D");
	al_draw_text(fuente, al_map_rgb(0, 0, 255), 945, 180, ALLEGRO_ALIGN_LEFT, "Disparo:");
	al_draw_text(fuente, al_map_rgb(0, 255, 0), 955, 210, ALLEGRO_ALIGN_LEFT, "J,I,K,L");
	al_draw_text(fuente, al_map_rgb(0, 0, 255), 955, 260, ALLEGRO_ALIGN_LEFT, "Piloto");
	al_draw_text(fuente, al_map_rgb(0, 0, 255), 925, 290, ALLEGRO_ALIGN_LEFT, "Automatico:");
	al_draw_text(fuente, al_map_rgb(0, 255, 0), 985, 320, ALLEGRO_ALIGN_LEFT, "M");
	al_flip_display();
}


//dibujarSalud: función encargada de dibujar en pantalla la salud actual
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void dibujarSalud(){
	char saludCadena[10];
	sprintf(saludCadena, "%.2f", personaje->salud);
	al_draw_text(fuente, al_map_rgb(0, 0, 255), 75, 25, ALLEGRO_ALIGN_CENTRE, "Salud:");
	al_draw_text(fuente, al_map_rgb(0, 255, 0), 75, 55, ALLEGRO_ALIGN_CENTRE, saludCadena);
	al_flip_display();
}

//dibujarVelocidad: función encargada de dibujar en pantalla el la velovidad
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void dibujarVelocidad(){
	char velCadena[10];
	sprintf(velCadena, "%.f", velocidad);
	al_draw_text(fuente, al_map_rgb(0, 0, 255), 75, 105, ALLEGRO_ALIGN_CENTRE, "Velocidad:");
	al_draw_text(fuente, al_map_rgb(0, 255, 0), 75, 135, ALLEGRO_ALIGN_CENTRE, velCadena);
	al_flip_display();
}


//dibujarVidas: función encargada de dibujar en pantalla las vidas actuales
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void dibujarVidas(){
	char vidasCadena[10];
	sprintf(vidasCadena, "%i", personaje->vidas);
	al_draw_text(fuente, al_map_rgb(0, 0, 255), 75, 185, ALLEGRO_ALIGN_CENTRE, "Vidas:");
	al_draw_text(fuente, al_map_rgb(0, 255, 0), 75, 215, ALLEGRO_ALIGN_CENTRE, vidasCadena);
	al_flip_display();
}


//dibujarRectanguloJuego: función encargada de dibujar en pantalla el delimitador entre los enemigos triángulo y los demás componentes del juego
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void dibujarRectanguloJuego(){
	al_set_target_bitmap(al_get_backbuffer(pantalla));
	al_draw_filled_rectangle(0, posicionRectanguloJuego, anchoPantalla, posicionRectanguloJuego + 5, al_map_rgb(0, 255, 0));
	al_flip_display();
}


//dibujarBonus: función encargada de dibujar en pantalla los bonus disponibles
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: se evalúa si hay bonus activados en el array (valor diferente de NULL)
void dibujarBonus(){
	for (int i = 0; i < numeroBonus; i++){
		if (bonus[i] != NULL){
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
void dibujarPrincipal(int x, int y, int direccion){

	switch (direccion){
	case A: //en el caso de que la tela presionada sea la A, lo mueve a la izquierda
		if (personaje->salud > 0 && personaje->salud <= 100){ // mientras que la salus del personaje este entre cero y cien, osea, que todavia tenga salud
			al_draw_bitmap(principalIzquierda, x, y, NULL); // diuja al personaje cambiando la posicion x,y
			ultimo = principalIzquierda;
		}

		break;

	case D: // en elcaso de que la letra presionada sea la D, lo mueve a la derecha
		if (personaje->salud > 0 && personaje->salud <= 100){
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
void dibujarcarretera(){
	al_set_target_bitmap(carretera);

	al_set_target_bitmap(al_get_backbuffer(pantalla));
	al_draw_bitmap(carretera, 300, 83, NULL);
	al_flip_display();

}

//dibujarEnemigoTriangulo: función encargada de dibujar en pantalla al enemigo triángulo 
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void dibujarEnemigoTriangulo(){
	for (int i = 0; i < numeroEnemigosTriangulo; i++){

		if (enemigosTriangulo[i] != NULL){
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
void dibujarBalasEnemigoTriangulo(){
	for (int i = 0; i < numeroBalasEnemigo; i++){
		if (balasEnemigo[i] != NULL){
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
void dibujarBalasPersonaje(){
	for (int i = 0; i < numeroBalasPersonaje; i++){

		if (balasPersonaje[i] != NULL){

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
void dibujarMejoresPuntajes(){
	char puntajeCadena[10];
	char posicionCadena[10];
	for (int i = 0; i < mejoresPuntajesJuego; i++){
		sprintf(puntajeCadena, "%i", mejoresPuntajes[i]);
		sprintf(posicionCadena, "%i", i + 1);

		al_draw_text(fuente, al_map_rgb(0, 255, 0), 20, i * 30, ALLEGRO_ALIGN_CENTRE, posicionCadena);
		al_draw_text(fuente, al_map_rgb(0, 255, 0), 35, i * 30, ALLEGRO_ALIGN_CENTRE, " - ");
		al_draw_text(fuente, al_map_rgb(0, 0, 255), 70, i * 30, ALLEGRO_ALIGN_CENTRE, puntajeCadena);

	}
	al_flip_display();
}


//limpiarPantalla: función que limpia el elemento display creado al inicio del programa
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void limpiarPantalla(){
	al_clear_to_color(transparente);
}


//limpiarPuntajes: función encargada de setear en 0 los elementos del array de puntajes
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void limpiarPuntajes(){
	for (int i = 0; i < mejoresPuntajesJuego; i++){
		mejoresPuntajes[i] = 0;
	}
}


//guardarPuntajes: función encargada de guardar en un txt los puntajes obtenidos en el juego
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void guardarPuntajes(){
	documento = fopen("Puntajes.txt", "a");
	fprintf(documento, "%i \n", puntaje);
	fclose(documento);

}


//cargarPuntajes: función encargada de cargar los puntajes obtenidos en el juego
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void cargarPuntajes(){
	char puntajeTxt[10];
	int punt;
	int aux;
	documento = fopen("Puntajes.txt", "r");
	while (!feof(documento)){
		fgets(puntajeTxt, 10, documento);
		punt = atoi(puntajeTxt);
		for (int i = 0; i < mejoresPuntajesJuego; i++){
			if (punt >= mejoresPuntajes[i]){
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
void sumarPuntaje(int suma){
	puntaje += suma;
}

void restargas(int resta){
	gas -= resta;
}

void sumargasusado(int suma){
	gasusado += suma;
}


//sumarSalud: función encargada de sumar valor a la salud del personaje
//Entradas: el valor que se le suma a la salud
//Salidas: ninguna
//Restricciones: si la suma es mayor a 100.0, se le asigna 100.0 a la salud
void sumarSalud(float salud){
	personaje->salud += salud;
	if (personaje->salud > 100.0) personaje->salud = 100.0;
}


//sumarVida: función encargada de sumar un valor a las vidas del personaje
//Entradas: vida que se suma
//Salidas: ninguna
//Restricciones: ninguna
void sumarVida(int vida){
	personaje->vidas += vida;
}


//restarSalud: función encargada de restar salud al personaje principal
//Entradas: el daño que se restará a la salud
//Salidas: ninguna
//Restricciones: ninguna
void restarSalud(float danno){
	personaje->salud -= danno;
}


//restarVidas: función encargadas de restar vidas al personaje principal
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void restarVidas(){
	personaje->vidas -= 1;
}


//restaurarSalud: función encargada de restaurar toda la salud del personaje principal
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void restaurarSalud(){
	personaje->salud = 100.0;
}


//sinSalud: función encargada de evaluar si el personaje principal tiene o no salud
//Entradas: ninguna
//Salidas: true, si no tiene salud false: si aún tiene salud
//Restricciones: se evalúa si la salud es menor o igual a 0
bool sinSalud(){
	if (personaje->salud <= 0.0) return true;

	else return false;
}


//sinVidas: función encargada de evaluar si el personaje principal tiene vidas o no
//Entradas: ninguna
//Salidas: true: si aún posee vidas false: si no posee vidas
//Restricciones: se evalúa si la cantidad de vidas es igual a 0
bool sinVidas(){
	if (personaje->vidas == 0) return true;
	else return false;
}


//cambiarSpriteEnemigoTriangulo: función encargada de sumar en uno la variable split del enemigo triángulo y moverlo aleatoriamente por la pantalla
/*el enemigo triangulo en realidad es invisible en el juego pero se mueve en la parte superior de la pantalla
para estar lanzando al azar sus balas, en este caso, los autos rojos que avanzan contra el personaje
*/
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: si el sumarSprite es 0, se suma en uno el sprite del enemigo triángulo
void cambiarSpriteEnemigoTriangulo(){

	for (int i = 0; i < numeroEnemigosTriangulo; i++){

		if (enemigosTriangulo[i] != NULL){

			if (enemigosTriangulo[i]->sprite == 1) enemigosTriangulo[i]->sumando = false;

			if (enemigosTriangulo[i]->sprite == 0) enemigosTriangulo[i]->sumando = true;

			if (enemigosTriangulo[i]->sumando){
				if (enemigosTriangulo[i]->sumarSprite == 0){
					enemigosTriangulo[i]->sprite += 1;
					enemigosTriangulo[i]->sumarSprite = 30;
				}

				else enemigosTriangulo[i]->sumarSprite -= 1;
			}

			if (!enemigosTriangulo[i]->sumando){
				if (enemigosTriangulo[i]->sumarSprite == 0){
					enemigosTriangulo[i]->sprite -= 1;
					enemigosTriangulo[i]->sumarSprite = 30;
				}

				else enemigosTriangulo[i]->sumarSprite -= 1;
			}
		}
	}
}


//generarBonus: función encargada de crear un bonus en el juego (se agrega al array de bonus)
//este genera un bonus en una posicion de pantalla ya determinada
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: se evalúa si se posee espacio en el array (valor NULL)
void generarBonus(){
	int x;
	int y;

	for (int i = 0; i < numeroBonus; i++){
		if (bonus[i] == NULL){
			x = 33 + 0;
			y = rand() % 6 + 7;
			bonus[i] = new Bonus(x * 10, y * 40, i % 2, saludBonus);
			bonusgenerados++;
			bonusperdidos++;
			break;
		}
	}
}


//crearBalaEnemigoTriangulo: función encargada de crear una nueva bala en el array de balas del enemigo
//Entradas: eje x, eje y
//Salidas: ninguna
//Restricciones: se evalúa si hay un valor desactivado (NULL) para asignar la bala
void crearBalaEnemigoTriangulo(int x, int y){
	for (int i = 0; i < numeroBalasEnemigo; i++){
		if (balasEnemigo[i] == NULL){
			balasEnemigo[i] = new BalaEnemigo(x, y, dannoBalaEnemigo);
			break;
		}
	}
}


//desactivarBalaPersonaje: función encargada de dejar un espacio libre en el array de balas
//Entradas: posición del array que se liberará
//Salidas: ninguna
//Restricciones: ninguna
void desactivarturboPersonaje(float turb){

	if (turb == true){
		//cambiovelocidad();
		turb = false;
	}
}


//desactivarBonus: función encargada de desactivar el bonus del array (hacerlo NULL)
//Entradas: la posición del array que se elimina
//Salidas: ninguna
//Restricciones: ninguna
void desactivarBonus(int posicion){
	bonus[posicion] = NULL;
}


//desactivarBalaEnemigoTriangulo: función encargada de desactivar balas del array de balas de enemigos
//Entradas: posición
//Salidas: ninguna
//Restricciones: ninguna
void desactivarBalaEnemigoTriangulo(int posicion){

	balasEnemigo[posicion] = NULL;
}

//desactivarBalaPersonaje: función encargada de dejar un espacio libre en el array de balas
//Entradas: posición del array que se liberará
//Salidas: ninguna
//Restricciones: ninguna
void desactivarBalaPersonaje(int posicion){

	balasPersonaje[posicion] = NULL;
}

//turbo: función encargada de activar el turbo
//Entradas: la dirección (UP, DOWN, LEFT, RIGHT)
//Salidas: ninguna
//Restricciones: se evalúa si una tecla fue activada
void turboPersonaje(float direccion){
	if (direccion == true){
		//cambiovelocidad(15);
		int a = 1;
	}
}

//dispararEnemigoTriangulo: función encargada de llamar a la función para crear balas del enemigo en caso de cumplirse cierta condición
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: se evalúa que haya enemigos activados (diferente de NULL), se evalúa si se cumple el rango para lanzar la bala
void dispararEnemigoTriangulo(){
	for (int i = 0; i < numeroEnemigosTriangulo; i++){

		if (enemigosTriangulo[i] != NULL){

			if (abs(enemigosTriangulo[i]->x - personaje->x) == 50){
				crearBalaEnemigoTriangulo(enemigosTriangulo[i]->x, enemigosTriangulo[i]->y);
				reproducirDisparoEnemigo();
				numenemigos++;
			}

		}
	}
}

//reproducirDisparoPersonaje: función encargada de reproducir el sonido para el disparo del personaje
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void reproducirDisparoPersonaje(){
	instanciaSonido = al_create_sample_instance(disparoPersonaje);
	al_set_sample_instance_playmode(instanciaSonido, ALLEGRO_PLAYMODE_ONCE);
	al_attach_sample_instance_to_mixer(instanciaSonido, al_get_default_mixer());
	al_play_sample_instance(instanciaSonido);
}

//dispararPersonaje: función encargada de crear nuevas balas
//Entradas: la dirección (UP, DOWN, LEFT, RIGHT)
//Salidas: ninguna
//Restricciones: se evalúa si existe algún espacio disponible en el array (o sea, si es NULL)
void dispararPersonaje(int direccion){
	if (direccion == S || direccion == W || direccion == D || direccion == A){

		for (int i = 0; i < numeroBalasPersonaje; i++){

			if (balasPersonaje[i] == NULL){
				balasPersonaje[i] = new BalaPersonaje(personaje->x, personaje->y, direccion);
				reproducirDisparoPersonaje();
				balasdisp++;
				balasperdidas++;
				break;

			}
		}
	}
}



//moverEnemigoTriangulo: función encargada de sumar los ejer x,y del enemigo triángulo
//Entradas: movimiento (lo que se le suma a los ejes), tiempo (si es segundo o tercer timer)
//Salidas: ninguna
//Restricciones: se evalúa si el enemigo está al punto mínimo o máximo del eje x, para así cambiar el rumbo//<770 >300
void moverEnemigoTriangulo(int movimiento, int tiempo){
	for (int i = 0; i < numeroEnemigosTriangulo; i++){

		if (enemigosTriangulo[i]->tiempo == tiempo){

			if (enemigosTriangulo[i]->direccion == A){
				if (enemigosTriangulo[i]->x == 300) enemigosTriangulo[i]->direccion = D;
				else enemigosTriangulo[i]->x -= movimiento;
			}

			if (enemigosTriangulo[i]->direccion == D){
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
void moverPersonaje(int movimiento){
	if (teclasDireccion[S]){
		if (personaje->y < 500) personaje->y += movimiento;
	}
	if (teclasDireccion[W]){
		if (personaje->y > posicionRectanguloJuego + 10) personaje->y -= movimiento;
	}
	if (teclasDireccion[D]){
		if (personaje->x < (770)) personaje->x += movimiento;
	}
	if (teclasDireccion[A]){
		if (personaje->x > 300) personaje->x -= movimiento;
	}
}

//cambiar velocidad: función encargada de cambiar la velocidad del personaje principal
//Entradas: movimiento (para saber si se suma o resta velocidad)
//Salidas: ninguna
//Restricciones: ninguna
void cambiovelocidad(int movimiento){
	if (teclasDireccion[S]){ // si se presiona la letra S se le va a empezar a restar velocidad cada vez que se presione o si se mantiene presionda
		velocidad-=2;
	}
	if (teclasDireccion[W]){// si se presiona la letra W se le va a empezar a sumar velocidad cada vez que se presione o si se mantiene presionda
		velocidad+=2;
	}
}


//moverBalaPersonaje: función encargada de cambiar el valor de los ejes de las balas disponibles en el array
//Entradas: movimiento (valor que se sumará a los ejes)
//Salidas: ninguna
//Restricciones: se evalúa si hay alguna bala acticava, además de llamar a la función para desactivarla si llega a los ejes límite
void moverBalaPersonaje(int movimiento){
	for (int i = 0; i < numeroBalasPersonaje; i++){

		if (balasPersonaje[i] != NULL){

			if (balasPersonaje[i]->orientacion == S){
				if (balasPersonaje[i]->y == (largoPantalla - 30)) desactivarBalaPersonaje(i);
				else balasPersonaje[i]->y += movimiento;
			}

			else if (balasPersonaje[i]->orientacion == W){
				if (balasPersonaje[i]->y == 60) desactivarBalaPersonaje(i);
				else balasPersonaje[i]->y -= movimiento;
			}

			else if (balasPersonaje[i]->orientacion == D){
				if (balasPersonaje[i]->x == (anchoPantalla - 30)) desactivarBalaPersonaje(i);
				else balasPersonaje[i]->x += movimiento;
			}

			else if (balasPersonaje[i]->orientacion == A){
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
void moverBalaEnemigoTriangulo(int movimiento){
	for (int i = 0; i < numeroBalasEnemigo; i++){
		if (balasEnemigo[i] != NULL){
			if (balasEnemigo[i]->y == (largoPantalla - 30)) desactivarBalaEnemigoTriangulo(i);
			else balasEnemigo[i]->y += movimiento;
		}
	}
}


//colisionBalaEnemigoTriangulo: función encargada de evaluar si existe colisión entre alguna bala del enemigo y el personaje principal
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: se evalúa si hay balas activadas (diferente de NULL)
void colisionBalaEnemigoTriangulo(){
	for (int i = 0; i < numeroBalasEnemigo; i++){

		if (balasEnemigo[i] != NULL){
			if ((personaje->x - 15 < balasEnemigo[i]->x && balasEnemigo[i]->x < personaje->x + 15) && (personaje->y - 15 < balasEnemigo[i]->y && balasEnemigo[i]->y < personaje->y + 15)){
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
void colisionBalaconBala(){
	for (int i = 0; i < numeroBalasEnemigo; i++){

		if (balasEnemigo[i] != NULL && balasPersonaje[i] != NULL){
			if ((balasPersonaje[i]->x - 50 < balasEnemigo[i]->x && balasEnemigo[i]->x < balasPersonaje[i]->x + 50) && (balasPersonaje[i]->y - 50 < balasEnemigo[i]->y && balasEnemigo[i]->y < balasPersonaje[i]->y + 50)){
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
void colisionBonus(){
	for (int i = 0; i < numeroBonus; i++){
		if (bonus[i] != NULL){
			if ((personaje->x - 30 < bonus[i]->x  &&  bonus[i]->x < personaje->x + 30) && (personaje->y - 30 < bonus[i]->y && bonus[i]->y < personaje->y + 30)){

				if (bonus[i]->tipo == 0){
					sumarSalud(bonus[i]->salud);
					sumarPuntaje(sumaPuntajeBonusSalud);
					bonusop++;
					bonusperdidos--;
				}

				else if (bonus[i]->tipo == 1){
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


//iniciarPuntajes: función encargada de inicializar el array de puntajes en 0
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void iniciarPuntajes(){
	for (int i = 0; i < mejoresPuntajesJuego; i++){
		mejoresPuntajes[i] = 0;
	}
}


//iniciarPersonaje: función encargada de iniciar el personaje principal del juego
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void iniciarPersonaje(){
	personaje = new PersonajePrincipal(300, 500, 3, 100.0);
}


//iniciarEnemigosTriangulo: función encargada de iniciar los enemigos triángulo del juego
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void iniciarEnemigosTriangulo(){
	int x;

	for (int i = 0; i < numeroEnemigosTriangulo; i++){
		x = rand() % 31 + 5;
		enemigosTriangulo[i] = new EnemigoTriangulo(300, 0, (i % 2 + 2), i + 1, 0, dannoEnemigoTriangulo);
	}

}


//iniciarBalasPersonaje: función encargada de iniciar las balas del personaje principal del juego
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void iniciarBalasPersonaje(){
	for (int i = 0; i < numeroBalasPersonaje; i++){
		balasPersonaje[i] = NULL;
	}
}


//iniciarBalasEnemigo: función encargada de iniciar las balas del enemigo del juego
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void iniciarBalasEnemigo(){
	for (int i = 0; i < numeroBalasEnemigo; i++){
		balasEnemigo[i] = NULL;
	}
}


//iniciarBonus: función encargada de iniciar los bonus del juego
//Entradas: ninguna
//Salidas: ninguna
//Restricciones: ninguna
void iniciarBonus(){
	for (int i = 0; i < numeroBonus; i++){
		bonus[i] = NULL;
	}
}


int main(int argc, char **argv){



	if (!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}

	//Esta línea de código permite que la ventana tenga la capacidad de cambiar de tamaño
	al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE);

	//crea la pantalla del juego
	pantalla = al_create_display(anchoPantalla, largoPantalla);
	al_set_window_position(pantalla, 100, 50);
	al_set_window_title(pantalla, "Auto de Carreras");
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

	fuente = al_load_font("8bitoperator_jve.ttf", 30, NULL);


	al_init_primitives_addon();

	//Línea para obtener las funcionalidades de las imágenes
	//*******************
	al_init_image_addon();
	//*******************

	//Se crean los contenedores de las imágenes que se utilizarán en el juego
	//*******************
	principalBuffer = al_create_bitmap(30, 30);
	enemigoTrianguloBuffer = al_create_bitmap(60, 60);
	carretera = al_create_bitmap(20, 30);
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
	carretera = al_load_bitmap("Imagenes/carretera2.png");
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
	while (menu){
		al_wait_for_event(colaEventoMenu, &eventoMenu);

		if (eventoMenu.type = ALLEGRO_EVENT_KEY_DOWN){

			if (eventoMenu.keyboard.keycode == ALLEGRO_KEY_1){
				hecho = true;
				menu = false;
			}

			else if (eventoMenu.keyboard.keycode == ALLEGRO_KEY_2){
				limpiarPantalla();
				cargarPuntajes();
				dibujarMejoresPuntajes();
				limpiarPuntajes();
			}

			else if (eventoMenu.keyboard.keycode == ALLEGRO_KEY_3){
				hecho = false;
				menu = false;
			}

			else if (eventoMenu.keyboard.keycode == ALLEGRO_KEY_S){
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
	//**********************************************************

	//Llamado a las funciones que inicializan los componentes lógicos del juego
	//**********************************************************
	iniciarPersonaje();
	iniciarEnemigosTriangulo();
	iniciarBalasPersonaje();
	iniciarBalasEnemigo();
	iniciarBonus();
	//**********************************************************


	while (hecho){

		ALLEGRO_EVENT eventos;

		al_wait_for_event(colaEventos, &eventos);


		/*Evento que toma en cuenta la tecla más actual presionada, se van activando en un array que representa cada movimiento
		Observación: Si se presiona una segunda tecla, al mismo tiempo que se está presionando la primera, este evento será capaz de detectar las dos teclas
		y representarlas en el array de activación (teclas)
		*/
		if (eventos.type == ALLEGRO_EVENT_KEY_DOWN){
			switch (eventos.keyboard.keycode){

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

			case ALLEGRO_KEY_K:
				disparo = S;
				//turb = true;
				break;

			case ALLEGRO_KEY_L:
				disparo = D;
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
		if (eventos.type == ALLEGRO_EVENT_KEY_UP){
			switch (eventos.keyboard.keycode){

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

		if (eventos.type == ALLEGRO_EVENT_TIMER){
			if (eventos.timer.source == primerTimer){
				moverPersonaje(movimiento);
				cambiovelocidad(movimiento);

			}

			else if (eventos.timer.source == segundoTimer){
				moverEnemigoTriangulo(movimiento, 1);
			}

			else if (eventos.timer.source == tercerTimer){
				moverEnemigoTriangulo(movimiento, 2);

			}

			else if (eventos.timer.source == cuartoTimer){
				for (int s = 0; s < 2; s++){
					turboPersonaje(turb);
				}
				dispararPersonaje(disparo);
				disparo = -1;
				moverBalaPersonaje(movimiento);
				dispararEnemigoTriangulo();
				moverBalaEnemigoTriangulo(movimiento);
				if (turb == true){
					for (int j = 0; j < 1; j++){
						desactivarturboPersonaje(turb);
					}
				}
			}

			else if (eventos.timer.source == quintoTimer){
				sumarPuntaje(sumaPuntajeTimer);
				restargas(restagastimer);
				sumargasusado(sumagasusadotimer);
			}

			else if (eventos.timer.source == sextoTimer){
				generarBonus();
			}

			else if (eventos.timer.source == septimoTimer){
				dibujarcarretera();
				dibujarPrincipal(personaje->x, personaje->y, direccion);
				dibujarEnemigoTriangulo();
				dibujarBalasPersonaje();
				dibujarBalasEnemigoTriangulo();
				dibujarBonus();
				//dibujarRectanguloJuego();
				dibujarPuntaje();
				dibujargas();
				dibujargasusado();
				dibujarSalud();
				dibujarVidas();
				dibujarVelocidad();
				//dibujarbalasdisp();
				dibujarcontroles();
				//dibujarnumbonus();
				//dibujarenemigosel();
				//dibujarbalasperdidas();
				//dibujarnumenemigos();
				//dibujarbonusgenerados();
				//dibujarbonusperdidos();

			}

			else if (eventos.timer.source == octavoTimer){
				moverEnemigoTriangulo(movimiento, 3);
			}

		}

		cambiarSpriteEnemigoTriangulo();
		limpiarPantalla();
		colisionBalaconBala();
		colisionBalaEnemigoTriangulo();
		colisionBalaconBala();
		colisionBonus();

		if (sinSalud()){
			restarVidas();
			restaurarSalud();
		}

		if (sinVidas()){
			hecho = false;
			guardarPuntajes();
		}

		if (gas == 0){
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

	al_destroy_sample(musicaJuego);
	al_destroy_sample(turbPersonaje);
	al_destroy_sample(disparoEnemigo);
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

	al_destroy_font(fuente);

	return 0;
}
