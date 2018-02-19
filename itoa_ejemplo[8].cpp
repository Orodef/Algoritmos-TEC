// itoa_ejemplo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h>  
#include <string.h>  


const int MAX = 100;

int main(void)
{
	char Texto[65];
	int contador;
	for (contador = 0; contador < MAX; contador++)
	{
		_itoa_s(contador, Texto, 65, 10);
		
		char NombreAutomatico[65] = "Pieza_";
		strcat_s(NombreAutomatico, Texto);
		
		printf("Para el entero %d: el texto es  %s\n", contador, NombreAutomatico );
	}
	
	system("pause");
	
}