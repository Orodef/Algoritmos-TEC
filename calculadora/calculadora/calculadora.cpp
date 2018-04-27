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

void InicializarLista(PtrOperador &Lista)//crear lista
{
	Lista = NULL;
}
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
PtrOperador CrearOperador(int &i, char Ncaracter)	//crearArticulo
{
	PtrOperador Operador = new(TOperador);
	//char buffer[5];

	Operador->caracter = Ncaracter;

	//strcpy_s(Pieza->Nombre, "Pieza");
	//_itoa_s(NCodigo, buffer, 10);
	//strcat_s(Pieza->Nombre, buffer);

	Operador->Siguiente = NULL;
	i++;
	return Operador;
}
void AgregarInicioLista(PtrOperador &Lista, PtrOperador &Nuevo)
{
	Nuevo->Siguiente = Lista;
	Lista = Nuevo;
}
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
		//Lista = Aux;
	}
	else
	{
		Lista = Nuevo;
	}


}

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
/*PtrOperador BuscarPila(PtrOperador &Lista, int cual)
{
return Lista;
}*/


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

void Push(PtrOperador &Lista, PtrOperador Nuevo) {
	AgregarInicioLista(Lista, Nuevo);
}
PtrOperador Top(PtrOperador &Lista) {
	if (Lista != NULL) {
		printf("%d ", Lista->caracter);
	}
	return Lista;
}
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

bool parentesisEquivalentes(char linea[]) {
	PtrOperador Aux;
	PtrOperador Nuevo;
	int longitud;
	int i;
	char caracter;//global?

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

bool operador(char caracter) {
	if ((caracter == '*') || (caracter == '/') || (caracter == '-') || (caracter == '+') || (caracter == '^')) {
		return true;
	}
	else
		return false;
}

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

void InicializarLista2(PtrOperando &Lista)//crear lista
{
	Lista = NULL;
}
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
PtrOperando CrearOperando(int &i, double Nvalor)	//crearArticulo
{
	PtrOperando Operando = new(TOperando);
	//char buffer[5];

	Operando->valor = Nvalor;

	//strcpy_s(Pieza->Nombre, "Pieza");
	//_itoa_s(NCodigo, buffer, 10);
	//strcat_s(Pieza->Nombre, buffer);

	Operando->Siguiente = NULL;
	i++;
	return Operando;
}
void AgregarInicioLista2(PtrOperando &Lista, PtrOperando &Nuevo)
{
	Nuevo->Siguiente = Lista;
	Lista = Nuevo;
}
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
		//Lista = Aux;
	}
	else
	{
		Lista = Nuevo;
	}


}

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

void GuardarLista2(PtrOperando Lista) {
	FILE *archivo;
	fopen_s(&archivo, "ARCHIVO2.txt", "w+");
	if (NULL == archivo) {
		printf("No se pudo abrir el archivo. \n");
	}
	else {
		PtrOperando AUX = Lista;
		while (AUX != NULL) {
			fprintf(archivo, "%i\n", AUX->valor);
			AUX = AUX->Siguiente;
		}


	}
	fclose(archivo);
}
void CargarLista2(PtrOperando &Lista) {
	PtrOperando Nuevo;
	FILE *archivo;
	fopen_s(&archivo, "ARCHIVO2.txt", "r");
	if (NULL == archivo) {
		printf("No se pudo abrir el archivo. \n");
	}
	else {


		while (!feof(archivo)) {
			Nuevo = new(TOperando);
			//char Cadena[20];
			fscanf_s(archivo, "%d\n", &Nuevo->valor);
			//fscanf_s(archivo, "%s\n", Cadena, 20);
			//strcpy_s(Nuevo->Nombre, Cadena);
			Nuevo->Siguiente = NULL;
			AgregarFinalLista2(Lista, Nuevo);

		}
	}
	fclose(archivo);
}

//Para pilas PtrOperando
//******************************************

void Push2(PtrOperando &Lista, PtrOperando Nuevo) {
	AgregarInicioLista2(Lista, Nuevo);
}
PtrOperando Top2(PtrOperando &Lista) {
	if (Lista != NULL) {
		printf("%d ", Lista->valor);
	}
	return Lista;
}
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

double sumar(double x, double y) {
	double suma;
	suma = x + y;
	return suma;
}

double restar(double x, double y) {
	double resta;
	resta = x - y;
	return resta;
}

double multiplicar(double x, double y) {
	double producto;
	producto = x * y;
	return producto;
}

double dividir(double x, double y) {
	double cociente;
	cociente = x / y;
	return cociente;
}

double elevar(double x, double y) {
	double resultado;
	resultado = pow(x, y);//pow se utiliza para elevar una base(x) a un exponente(y)http://www.cplusplus.com/reference/cmath/pow/
	return resultado;
}

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
