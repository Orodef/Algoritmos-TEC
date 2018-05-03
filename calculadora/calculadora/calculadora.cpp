// CALCULADORA por pilas
//Luciano Cendra Villalobos y Alejandro Hernández Lobo

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <math.h>	//para funcion pow

using namespace std;

//******************************************
int x;					 //parametro global

						 //******************************************
typedef struct TOperador		//TArticulo
{
	char caracter;
	TOperador *Siguiente;
}*PtrOperador;

typedef struct TOperando		//TArticulo
{
	double valor;
	TOperando *Siguiente;
}*PtrOperando;

//******************************************
//FUNCIONES
//*************************************************************

//Para TOperador
//*************************************************************

//InicializarLista: función encargada de inicializar una lista
//Entradas: PtrOperador &Lista lo toma por referencia porque hara cambios directamente en el mismo
//Salidas: ninguna
//Restricciones: ninguna
void InicializarLista(PtrOperador &Lista)//crear lista
{
	Lista = NULL;
}

//DestruirLista: función encargada de destruir una lista
//Entradas: PtrOperador &Lista lo toma por referencia porque hara cambios directamente en el mismo
//Salidas: ninguna
//Restricciones: ninguna
void DestruirLista(PtrOperador &Lista)
{
	PtrOperador Aux;
	Aux = Lista;
	while (Aux != NULL)
	{
		Lista = Lista->Siguiente;
		delete(Aux);
		Aux = Lista;
	}
}

//CrearOperador: función encargada de crear un operador, crea un operador por medio de un puntero. Los operadores en las expresiones matematicas de este protecto son: +, *, /, -, ^
//Entradas: char Ncaracter sera utilizado como valor para el nuevo operador que es una variable anonima
//Salidas: retorna el operador nuevo, que es una variable anonima de tipo PtrOperador
//Restricciones: ninguna
PtrOperador CrearOperador(char Ncaracter)	
{
	PtrOperador Operador = new(TOperador);

	Operador->caracter = Ncaracter;


	Operador->Siguiente = NULL;
	return Operador;
}

//AgregarInicioLista: función encargada de agregar al inicio de una lista
//Entradas: PtrOperador &Lista lo toma por referencia porque hara cambios directamente en el mismo, PtrOperador &Nuevo que sera el operador que se agregara a la lista
//Salidas: ninguna
//Restricciones: ninguna
void AgregarInicioLista(PtrOperador &Lista, PtrOperador &Nuevo)
{
	Nuevo->Siguiente = Lista;
	Lista = Nuevo;
}

//AgregarFinalLista: función encargada de agregar al final de una lista
//Entradas: PtrOperador &Lista lo toma por referencia porque hara cambios directamente en el mismo, PtrOperador &Nuevo que sera el operador que se agregara a la lista
//Salidas: ninguna
//Restricciones: ninguna
void AgregarFinalLista(PtrOperador &Lista, PtrOperador &Nuevo)
{
	PtrOperador Aux;
	Aux = Lista;
	if (Aux != NULL)
	{
		while (Aux->Siguiente != NULL)
		{
			Aux = Aux->Siguiente;
		}

		Aux->Siguiente = Nuevo;
	}
	else
	{
		Lista = Nuevo;
	}


}

//Listar: función encargada de imprimir a pantalla los contenidos de una lista. No imprime directamente de ella, sino que imprime el valor asociado a cada elemento (operador) de ella
//Entradas: PtrOperador &Lista que es la lista a imprimir
//Salidas: ninguna
//Restricciones: ninguna
void Listar(PtrOperador &Lista)
{
	int Contador = 1;
	PtrOperador Aux;
	Aux = Lista;
	printf("Posfija: \n");
	while (Aux != NULL)
	{
		//printf(" %d ", Contador);
		printf("%c", Aux->caracter);
		Aux = Aux->Siguiente;
		Contador++;
	}
}

//GuardarLista: función encargada guardar de una lista en un archivo. Si este no existe, lo crea
//Entradas: PtrOperador Lista que es la lista a guardar
//Salidas: ninguna
//Restricciones: ninguna
void GuardarLista(PtrOperador Lista) {
	FILE *archivo;
	fopen_s(&archivo, "ARCHIVO.txt", "w+");
	if (NULL == archivo) {
		printf("No se pudo abrir el archivo. \n");
	}
	else {
		PtrOperador AUX = Lista;
		while (AUX != NULL) {
			fprintf(archivo, "%i\n", AUX->caracter);
			AUX = AUX->Siguiente;
		}


	}
	fclose(archivo);
}

//CargarLista: función encargada cargar una lista de un archivo que lee
//Entradas: PtrOperador Lista que es la lista a cargar
//Salidas: ninguna
//Restricciones: ninguna
void CargarLista(PtrOperador &Lista) {
	PtrOperador Nuevo;
	FILE *archivo;
	fopen_s(&archivo, "ARCHIVO.txt", "r");
	if (NULL == archivo) {
		printf("No se pudo abrir el archivo. \n");
	}
	else {


		while (!feof(archivo)) {
			Nuevo = new(TOperador);
			//char Cadena[20];
			fscanf_s(archivo, "%s\n", &Nuevo->caracter);
			//fscanf_s(archivo, "%s\n", Cadena, 20);
			//strcpy_s(Nuevo->Nombre, Cadena);
			Nuevo->Siguiente = NULL;
			AgregarFinalLista(Lista, Nuevo);

		}
	}
	fclose(archivo);
}

//Para pilas PtrOperador
//*************************************************************

//Push: función encargada de meter un nuevo elemento al tope de una pila
//Entradas: PtrOperador Lista que es la pila a la que se le agregara un elemento en su tope, PtrOperador Nuevo que es el elemento por agregar al tope
//Salidas: ninguna
//Restricciones: ninguna
void Push(PtrOperador &Lista, PtrOperador Nuevo) {
	AgregarInicioLista(Lista, Nuevo);
}

//Top: función encargada de indicar que elemento se encuentra en el tope de una pila
//Entradas: PtrOperador Lista que es la pila a la que se le analizara le tope
//Salidas: Lista, la cual no sufrio cambio alguno
//Restricciones: ninguna
PtrOperador Top(PtrOperador &Lista) {
	if (Lista != NULL) {
		printf("%d ", Lista->caracter);
	}
	return Lista;
}

//Pop: función encargada de sacar el elemento del tope de una pila
//Entradas: PtrOperador Lista que es la pila a la que se le sacara el elemento del tope, char &caracterOperador que es una variable en la que se guardara el valor del elemento del tope
//			esta variable es de suma importancia para la logica de la calculadora, ya que deja un valor tipo char que sera utilizado en el calculo de operaciones
//Salidas: Aux, la cual es una pila auxiliar, que termina sin el tope que tenia lista. Sera la nueva lista
//Restricciones: ninguna
PtrOperador Pop(char &caracterOperador, PtrOperador &Lista) {
	PtrOperador Aux = Lista;
	char Ncaracter;
	if (Aux != NULL) {
		Ncaracter = Aux->caracter;
		Lista = Lista->Siguiente;
		caracterOperador = Ncaracter;
		Aux->Siguiente = NULL;
	}
	return Aux;
}

//Para pasar de infijo a posfijo
//*************************************************************

//parentesisEquivalentes: función encargada de indicar si una expresion matematica tiene la misma cantidad de parentesis izquierdos que derechos
//Entradas: char linea[] que representa la expresion matematica a evaluar
//Salidas: un true, en caso de que haya misma cantidad de parentesis izquierdos que derechos, un false de caso contrario
//Restricciones: ninguna
bool parentesisEquivalentes(char linea[]) {
	PtrOperador Aux;
	PtrOperador Nuevo;
	int longitud;
	int i;
	char caracter;

	InicializarLista(Aux);
	InicializarLista(Nuevo);
	longitud = strlen(linea); //recibe un string y devuelve su longitud en tipo int
	for (i = 0; i < longitud; i++) {
		if (linea[i] == '(') {
			Nuevo = CrearOperador(x, linea[i]);
			Push(Aux, Nuevo);
		}
		if (linea[i] == ')')
		{
			Pop(caracter, Aux);
		}
	}
	if (Aux == NULL) {
		return true;
	}
	else
		return false;
}

//operador: función encargada de indicar si un caracter es operador o no (*,-,+,/,^)
//Entradas: char caracter, que representa el caracter a evaluar
//Salidas: un true, en caso de que el caracter sea operador, un false en caso contrario
//Restricciones: ninguna
bool operador(char caracter) {
	if ((caracter == '*') || (caracter == '/') || (caracter == '-') || (caracter == '+') || (caracter == '^')) {
		return true;
	}
	else
		return false;
}

//prioridad: función encargada de devolver que prioridad tiene un caracter(operador) que recibe
//Entradas: char operador, que representa un caracter que se evaluara
//Salidas: un 4 si es ^, un 3 si es * o /, un 2 si es + o -, y un 1 si es (
//Restricciones: splo trabaja con caracteres que sean operadores
int prioridad(char operador) {//prioridad del 1 al 4 para operadores
	if (operador == '^') {
		return 4;
	}
	if ((operador == '*') || (operador == '/')) {
		return 3;
	}
	if ((operador == '+') || (operador == '-')) {
		return 2;
	}
	if (operador == '(') {
		return 1;
	}
}

//infijo_a_posfijo: función de suma importancia para la logica de la calculadora encargada de pasar un arreglo de caracteres que representa una expresion matematica infija, a una
//					pila de caracteres que representan la expresion posfija
//Entradas: char linea[], arreglo de caracteres que representa la expresion matematica infija, ingresada por el usuario
//Salidas: PtrOperador posfijo, que es una pila de caracteres que representan la expresion posfija
//Restricciones: ninguna
PtrOperador infijo_a_posfijo(char linea[]) {
	PtrOperador posfijo;
	PtrOperador pila;// = CrearOperador(x, 3); pila temporal para operadores
	PtrOperador Nuevo;
	int longitud;
	int i;
	char caracter;

	InicializarLista(pila);
	InicializarLista(Nuevo);
	InicializarLista(posfijo);
	longitud = strlen(linea);

	for (i = 0; i < longitud; i++) {
		/*if (linea[i] == ' ') {

		}*/
		//verifico si es operando
		if (((linea[i] >= 48) && (linea[i] <= 57)) || ((linea[i] >= 65) && (linea[i] <= 90)) || ((linea[i] >= 97) && (linea[i] <= 122))) {//en ASCII, del 48 al 57 son numeros del 0 al 9 (fuente: https://ascii.cl/es/)
																																		  //del 65 al 90 son letras mayusculas(operandos como variables)
																																		  //del 97 al 122 son letras minusculas
			Nuevo = CrearOperador(x, char(linea[i]));
			AgregarFinalLista(posfijo, Nuevo); //Si el caracter es operando, entra de una vez al posfijo
		}
		//verifico si es parentesis derecho
		else if (linea[i] == ')') {
			while ((pila->caracter != '(') && (pila != NULL)) {//saca valores de la pila temporal y los mete al posfijo, hasta toparse un parentesis izquierdo
				Pop(caracter, pila);
				Nuevo = CrearOperador(x, caracter);
				AgregarFinalLista(posfijo, Nuevo);
			}
			//ahora, se quita el parentesis, pero no se agrega al posfijo
			Pop(caracter, pila);
		}
		else {
			if (linea[i] == '(') {
				Nuevo = CrearOperador(x, char(linea[i]));
				Push(pila, Nuevo);//Si el caracter es parentesis izquierdo, entra de una vez al posfijo
			}
			else {
				if (operador(linea[i])) {//evaluo si el caracter es operador
					if (pila == NULL) {
						Nuevo = CrearOperador(x, char(linea[i]));
						Push(pila, Nuevo);//Si la pila esta vacia, entra a la pila temporal
					}
					else {//en caso de que ya hayan operadores en la pila
						while (pila != NULL) {
							if ((prioridad(pila->caracter)) >= (prioridad(linea[i]))) {//evaluo si la prioridad del caracter en el tope es mayor o igual a la del operador que estoy analizando
								Pop(caracter, pila);//saco el tope de la pila, pero guardo el valor de su caracter
								Nuevo = CrearOperador(x, caracter);	//con el valor de caracter, creo un nuevo operador que ira en el posfijo
								AgregarFinalLista(posfijo, Nuevo);//lo que se ha hecho(en estas 3 lineas) es agarrar el tope de la pila y colocarlo en el posfijo
							}
							else
								break;
						}
						Nuevo = CrearOperador(x, char(linea[i]));
						Push(pila, Nuevo);//ya que se evaluo prioridad, se avanza a meter el operador analizado en la pila temporal
					}
				}
			}
		}
		//
	}
	//se vacia la pila temporal en el posfijo, en caso de que hayan quedado operadores
	while (pila != NULL) {
		Pop(caracter, pila);
		Nuevo = CrearOperador(x, caracter);
		AgregarFinalLista(posfijo, Nuevo);
	}
	return posfijo;
}

//Para TOperando
//*************************************************************

//se utilizan funciones iguales, pero que trabajan con el struct de los operandos, a diferencia de las otras que trabajan con struct de operadores

//InicializarLista2: función encargada de inicializar una lista
//Entradas: PtrOperando &Lista lo toma por referencia porque hara cambios directamente en el mismo
//Salidas: ninguna
//Restricciones: ninguna
void InicializarLista2(PtrOperando &Lista)//crear lista
{
	Lista = NULL;
}

//DestruirLista2: función encargada de destruir una lista
//Entradas: PtrOperando &Lista lo toma por referencia porque hara cambios directamente en el mismo
//Salidas: ninguna
//Restricciones: ninguna
void DestruirLista2(PtrOperando &Lista)
{
	PtrOperando Aux;
	Aux = Lista;
	while (Aux != NULL)
	{
		Lista = Lista->Siguiente;
		delete(Aux);
		Aux = Lista;
	}
}

//CrearOperando: función encargada de crear un operando, crea un operando por medio de un puntero. Los operadores en las expresiones matematicas de este protecto son: letras y numeros
//Entradas: char Ncaracter sera utilizado como valor para el nuevo operando que es una variable anonima
//Salidas: retorna el operando nuevo, que es una variable anonima de tipo PtrOperador
//Restricciones: ninguna
PtrOperando CrearOperando(int &i, double Nvalor)	//crearArticulo
{
	PtrOperando Operando = new(TOperando);
	Operando->valor = Nvalor;


	Operando->Siguiente = NULL;
	i++;
	return Operando;
}

//AgregarInicioLista2: función encargada de agregar al inicio de una lista
//Entradas: PtrOperando &Lista lo toma por referencia porque hara cambios directamente en el mismo, PtrOperando &Nuevo que sera el operando que se agregara a la lista
//Salidas: ninguna
//Restricciones: ninguna
void AgregarInicioLista2(PtrOperando &Lista, PtrOperando &Nuevo)
{
	Nuevo->Siguiente = Lista;
	Lista = Nuevo;
}

//AgregarFinalLista2: función encargada de agregar al final de una lista
//Entradas: PtrOperando &Lista lo toma por referencia porque hara cambios directamente en el mismo, PtrOperando &Nuevo que sera el operando que se agregara a la lista
//Salidas: ninguna
//Restricciones: ninguna
void AgregarFinalLista2(PtrOperando &Lista, PtrOperando &Nuevo)
{
	PtrOperando Aux;
	Aux = Lista;
	if (Aux != NULL)
	{
		while (Aux->Siguiente != NULL)
		{
			Aux = Aux->Siguiente;
		}

		Aux->Siguiente = Nuevo;
	}
	else
	{
		Lista = Nuevo;
	}


}

//Listar2: función encargada de imprimir a pantalla los contenidos de una lista. No imprime directamente de ella, sino que imprime el valor asociado a cada elemento (operando) de ella
//Entradas: PtrOperando &Lista que es la lista a imprimir
//Salidas: ninguna
//Restricciones: ninguna
void Listar2(PtrOperando &Lista)
{
	int Contador = 1;
	PtrOperando Aux;
	Aux = Lista;
	printf("Posfija: \n");
	while (Aux != NULL)
	{
		//printf(" %d ", Contador);
		printf("%d", Aux->valor);
		Aux = Aux->Siguiente;
		Contador++;
	}
}

//Para pilas PtrOperando
//******************************************

//Push2: función encargada de meter un nuevo elemento al tope de una pila
//Entradas: PtrOperando Lista que es la pila a la que se le agregara un elemento en su tope, PtrOperando Nuevo que es el elemento por agregar al tope
//Salidas: ninguna
//Restricciones: ninguna
void Push2(PtrOperando &Lista, PtrOperando Nuevo) {
	AgregarInicioLista2(Lista, Nuevo);
}

//Top2: función encargada de indicar que elemento se encuentra en el tope de una pila
//Entradas: PtrOperando Lista que es la pila a la que se le analizara le tope
//Salidas: Lista, la cual no sufrio cambio alguno
//Restricciones: ninguna
PtrOperando Top2(PtrOperando &Lista) {
	if (Lista != NULL) {
		printf("%d ", Lista->valor);
	}
	return Lista;
}

//Pop2: función encargada de sacar el elemento del tope de una pila
//Entradas: PtrOperando Lista que es la pila a la que se le sacara el elemento del tope, char &caracterOperador que es una variable en la que se guardara el valor del elemento del tope
//			esta variable es de suma importancia para la logica de la calculadora, ya que deja un valor tipo char que sera utilizado en el calculo de operaciones
//Salidas: Aux, la cual es una pila auxiliar, que termina sin el tope que tenia lista. Sera la nueva lista
//Restricciones: ninguna
PtrOperando Pop2(double &caracterOperando, PtrOperando &Lista) {
	PtrOperando Aux = Lista;
	double Nvalor;
	if (Aux != NULL) {
		Nvalor = Aux->valor;
		Lista = Lista->Siguiente;
		caracterOperando = Nvalor;
		Aux->Siguiente = NULL;
	}
	return Aux;
}

//Para calculadora
//******************************************

//sumar: función encargada sumar dos operandos
//Entradas: double x, operando 1, double y, operando 2
//Salidas: double suma, que es la suma de ambos
//Restricciones: ninguna
double sumar(double x, double y) {
	double suma;
	suma = x + y;
	return suma;
}

//restar: función encargada restar dos operandos
//Entradas: double x, operando 1, double y, operando 2
//Salidas: double resta, que es la resta de ambos
//Restricciones: ninguna
double restar(double x, double y) {
	double resta;
	resta = x - y;
	return resta;
}

//multiplicar: función encargada multiplicar dos operandos
//Entradas: double x, operando 1, double y, operando 2
//Salidas: double producto que es el producto de ambos
//Restricciones: ninguna
double multiplicar(double x, double y) {
	double producto;
	producto = x * y;
	return producto;
}

//dividir: función encargada dividir dos operandos
//Entradas: double x, operando 1, double y, operando 2
//Salidas: double cociente que es el cociente de ambos
//Restricciones: ninguna
double dividir(double x, double y) {
	double cociente;
	cociente = x / y;
	return cociente;
}

//elevar: función encargada elevar dos operandos, uno base, otro exponente
//Entradas: double x, operando 1 base, double y, operando 2 exponente
//Salidas: double resultado que es el resultado de ambos
//Restricciones: utiliza funcion pow
double elevar(double x, double y) {
	double resultado;
	resultado = pow(x, y);//pow se utiliza para elevar una base(x) a un exponente(y)http://www.cplusplus.com/reference/cmath/pow/
	return resultado;
}

//operacion: función encargada llevar a cabo una operacion matematica dependiendo de un operando
//Entradas: double x, operando 1, double y, operando 2, char operador, que definira la operacion por realizar
//Salidas: el resultado de cada funcion, dependiendo del operador: sumar, restar, multiplicar, dividir, elevar
//Restricciones: ninguna
double operacion(double operando1, double operando2, char operador) {
	switch (operador) {
	case '+':
		return sumar(operando1, operando2);
		break;
	case '-':
		return restar(operando1, operando2);
		break;
	case '*':
		return multiplicar(operando1, operando2);
		break;
	case '/':
		return dividir(operando1, operando2);
		break;
	case '^':
		return elevar(operando1, operando2);
		break;
	}
}

//evaluarPosfijo: función de alta importancia en la logica de la calculadora, encargada de evaluar un posfijo, que es una pila tipo PtrOperador, y hacer una serie de llamados a funciones
//				para dar un resultado final
//Entradas: PtrOperador Lista, que representa el posfijo
//Salidas: double resultadoFinal, que es el resultado final presentado al usuario

//Restricciones: solo trabaja con numeros de un digito, no trabaja calculos complejos como logaritmos

double evaluarPosfijo(PtrOperador Lista) {
	double resultado;
	double resultadoFinal;
	PtrOperando pila;
	PtrOperando Nuevo;
	PtrOperador Aux;
	char Ncaracter;
	double operando1;
	double operando2;

	InicializarLista2(pila);
	InicializarLista2(Nuevo);
	Aux = Lista;
	while (Aux != NULL)
	{
		if ((Aux->caracter >= 48) && (Aux->caracter <= 57)) {//numeros del 0 al 9 en ASCII
															 //((linea[i] >= 48) && (linea[i] <= 57)) || ((linea[i] >= 65) && (linea[i] <= 90)) || ((linea[i] >= 97) && (linea[i] <= 122))
			Ncaracter = Aux->caracter;
			Nuevo = CrearOperando(x, atof(&Ncaracter));//atof para convertir de tipo char a numero real http://algoritmos.victorgarro.com/lenguaje/ComandosC.htm
			Push2(pila, Nuevo);
		}
		else {
			if (operador(Aux->caracter)) {
				Pop2(operando2, pila);
				Pop2(operando1, pila);

				resultado = operacion(operando1, operando2, Aux->caracter);

				Nuevo = CrearOperando(x, resultado);
				Push2(pila, Nuevo);
			}
		}
		Aux = Aux->Siguiente;
	}
	resultadoFinal = pila->valor;
	return resultadoFinal;
}
//******************************************

//******************************************
//MAIN
//******************************************

int main()
{
	char expresionMate[50];
	PtrOperador Posfijo;
	InicializarLista(Posfijo);
	printf("Infijo: \n");
	cin >> expresionMate;

	if (parentesisEquivalentes(expresionMate)) {
		Posfijo = infijo_a_posfijo(expresionMate);
		Listar(Posfijo);
		printf("\nResultado: \n");
		printf("%f\n", evaluarPosfijo(Posfijo));

	}
	else
		printf("Expresion no valida\n");
	system("pause");
	DestruirLista(Posfijo);
	return 0;
}
//(6+4)*2/8^(7-4)   ejemplo de infijo proveido por usuario
