// EstructuraEstatica.cpp : Defines the entry point for the console application.
//

//importa las librerias
#include "StdAfx.h"
#include<iostream>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <conio.h>

using namespace std;
#pragma warning(disable:4996) //Quita los errores de "Function is insecure"
/*esta es la estructura de los datos q poseen los Productos*/
typedef struct T_Producto {
    int Codigo;
    char Descripcion[50];
    float Precio;
    int Disponible;
    bool Activo;
} X;

const int Max = 1000;  // Capacidad máxima de 1900
int Ultimo = 0;
void CargaAutomaticaDatosProducto(int NCodigo, int NDescripcion, float NPrecio, int NDisponible, T_Producto &Pieza)
{
    cout << "CodidoProducto:";
    char descrip[50];
    char texto[20] = "Producto_";

    Pieza.Codigo = NCodigo;

    _itoa_s(NDescripcion, descrip, 50, 10);
    strcat_s(texto, descrip);
    strcpy_s(Pieza.Descripcion, 80, texto);

    Pieza.Precio = NPrecio;
    Pieza.Disponible = NDisponible;
    Pieza.Activo = true;

}


void Listar(T_Producto Inventario[Max]) {  // Subrutina que lista todos  los prductos en el inventario
    system("CLS");
    int conta = 1;
    cout << "Listado de  Productos" << endl;
    for (int i = 1; i<Ultimo; i++) {  // Limita el listado a los espacios en inventario que sí contienen información relevante
        if (Inventario[i].Activo) {
            cout << Inventario[i].Codigo << " " << Inventario[i].Descripcion << " " << Inventario[i].Precio << " "
                << Inventario[i].Disponible << " " << endl;
            conta++;
            if (conta == 7) {  // Hace que se listen los objetos en grupos de máximo 7
                cout << "-----------------------------------" << endl;
                cout << "Presione cualquier tecla para continuar..." << endl;
                system("pause");
                system("CLS");

                conta = 1;
            }
        }
    }
    system("pause");
}



int BuscarElemento(T_Producto Inventario[Max], int Cual)  // Función que busca un elemnto en el inventario según su código de producto
{
    bool Encontro = false;
    int Cont = 0;

    while ((!Encontro) && (Cont<Ultimo))
    {
        if ((Inventario[Cont].Activo == true) && (Inventario[Cont].Codigo == Cual))
            Encontro = true;  // Cambia el valor a True si encuentra el objeto deseado
        else
            Cont++;
    }

    if (Encontro == true)
        return Cont;  // retorna la posición en inventario que tiene el objeto buscado
    else
        return -1;  // retorna -1 si mo lo encuentra

}
void InsertarElemento(T_Producto Inventario[Max], T_Producto Unidad)  // Subrutina que inserta un nuuevo elemento al arreglo Inventario
{

    Inventario[Ultimo] = Unidad;
    Ultimo++;
}

void BorrarElemento(T_Producto Inventario[Max], int Cual)  // Subrutina que cambia el estado de un producto en el inventario
{
    int PorBorrar;
    PorBorrar = BuscarElemento(Inventario, Cual);  // Llama a la función buscar elemento

    if (PorBorrar != -1)  // Si la función encuentra el objeto entra en el condicional
        Inventario[PorBorrar].Activo = false;  // Cambia el estado de un producto para que ya no aparezca más en el inventario

}


void main() {  // Menú que permite selccionar l aopción que se desea realizar

    T_Producto PiezasFerreteria[Max];
    T_Producto Elemento;

    int opcion = 0;

    while (opcion<4) {

        system("cls");//limpia la pantalla
        cout << "******Menu principal******" << endl;
        cout << "1. Cargar productos de prueba" << endl;
        cout << "2. Listar Piezas " << endl;
        cout << "3. Borrar Piezas " << endl;
        cout << "4. Salir " << endl;
        cin >> opcion;//captura el numero introducido

        switch (opcion) {  // Busca un caso que coincida con la opción seleccionada

        case 1:
            system("cls");
            for (int contador = 0; contador < 51; contador++) {
                CargaAutomaticaDatosProducto(contador, contador, contador, contador, Elemento);  // Llama a la función de carga automática para que genere elementos hasta el espacio 51
                InsertarElemento(PiezasFerreteria, Elemento);  // Inserta el elemento en el arreglo PiezasFerreteria
            }
            break;

        case 2:
            system("cls");
            Listar(PiezasFerreteria);
            break;
        case 3:
            system("CLS");
            int CodigoEscogido;
            cout << "Digite el codigo del Producto a Borrar" << endl;
            cout << "" << endl;
            cout << "CodidoProducto:" << endl;
            cin >> CodigoEscogido;
            BorrarElemento(PiezasFerreteria, CodigoEscogido);
            break;

        }
    }
}
