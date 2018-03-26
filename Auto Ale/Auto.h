// Struct que define al personaje principal, en este caso, al auto
// incluye la cantidad de vidas del personaje, la salud que tendra con cada vida y  la posicion en x,y

struct PersonajePrincipal{
	int x;
	int y;
	int vidas;
	float salud;

	PersonajePrincipal(){
	}

	PersonajePrincipal(int _x, int _y, int _vidas, float _salud){
		x = _x;
		y = _y;
		vidas = _vidas;
		salud = _salud;
	}

};