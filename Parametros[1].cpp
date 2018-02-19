#include "stdafx.h"
#include <conio.h>
#include <iostream> 

using namespace std;

//IMPLEMENTACION DE RUTINAS
//PARAMETROS FORMALES : Son las variables y sus tipos especificadas entre parentesis en el planteamiento de 
//                      un metodo o subrutina. En este caso n y m se ven como "variables locales" y constituyen
//                      una copia de las variables que se le pasen como parametros reales
int RutinaConParametrosxValor(int n, int m){
	n = n + 5;
	m = m - 5;
	return n + m;
}

//PARAMETROS FORMALES : Son las variables y sus tipos especificadas entre parentesis en el planteamiento de 
//                      un metodo o subrutina.  En este caso el "&" denota que n y m se van a comportar como "alias" 
//                      de las variables que se le pasen como parametros reales, es decir usaran la misma memoria
int RutinaConParametrosxReferencia(int &n, int &m){
	n = n + 5;
	m = m - 5;
	return n + m;
}




//PROGRAMA PRINCIPAL 
int main() {
	//EJEMPLO 1			
	//Empezamos haciendo a = 10 y b = 20, despuis llamamos a la funcisn "funcion" con las variables a y b como parametros. Dentro de "funcion" los parametros se llaman n y m, y cambiamos sus valores, sin embargo al retornar a "main", a y b conservan sus valores originales, 
	//esto sucede porque lo que pasamos no son las variables a y b, sino que copiamos sus valores a las variables n y m. 
	//en el ejemplo, la segunda llamada a "funcion". Los valores de los parametros no pueden cambiar al retornar de "funcion", ya que son constantes. 
	cout << "**************Ejemplo 1**************" << endl;
	int a, b;
	a = 10;
	b = 20;

	//PARAMETROS REALES : Son las variables o valores pasadas en el llamado a una subrutina 
	cout << "Valor de a,b antes del llamado a la rutina por valor->" << a << ", " << b << endl;
	cout << "RutinaConParametrosxValor(a,b) ->" << RutinaConParametrosxValor(a, b) << endl;
	cout << "Valor de a,b luego del llamado a la rutina por valor ->" << a << ", " << b << endl;
	cout << "RutinaConParametrosxValor(10,20) ->" << RutinaConParametrosxValor(10, 20) << endl;


	//EJEMPLO 2		
	//En este caso, las variables "a" y "b" tendran valores distintos despuis de llamar a la funcisn. Cualquier cambio que realicemos en los parametros dentro de la funcisn, se hara tambiin en las variables referenciadas. Esto quiere decir que no podremos llamar a la funcisn con parametros constantes.    
	cout << "**************Ejemplo 2**************" << endl;
	int d, e;
	d = 10; e = 20;

	//PARAMETROS REALES : Son las variables o valores pasadas en el llamado a una subrutina 
	cout << "Valor de d,e antes del llamado a la rutina por referencia->" << d << ", " << e << endl;
	cout << "d,e ->" << d << ", " << e << endl;
	cout << "RutinaConParametrosxReferencia(d,e) ->" << RutinaConParametrosxReferencia(d, e) << endl;
	cout << "Valor de d,e despues del llamado a la rutina por referencia->" << d << ", " << e << endl;
	/* cout << "funcion(10,20) ->" << funcion(10, 20) << endl;
	es ilegal pasar constantes como parametros cuando estos son referencias */


}







