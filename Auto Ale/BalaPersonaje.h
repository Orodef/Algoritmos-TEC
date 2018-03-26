
struct BalaPersonaje{
	int x;
	int y;
	int orientacion;
	bool activado;

	BalaPersonaje(){

	}

	BalaPersonaje(int _x, int _y, int _orientacion){
		x = _x;
		y = _y;
		orientacion = _orientacion;
		activado = false;
	}

};