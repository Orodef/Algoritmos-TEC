
struct EnemigoTriangulo{
	int x;
	int y;
	int direccion;
	int tiempo;
	int sprite;
	int sumarSprite;
	float danno;
	bool sumando;

	EnemigoTriangulo(){

	}

	EnemigoTriangulo(int _x, int _y, int _direccion, int _tiempo, int _sprite, float _danno){
		x = _x;
		y = _y;
		direccion = _direccion;
		tiempo = _tiempo;
		sprite = _sprite;
		sumarSprite = 30;
		danno = _danno;
		sumando = false;
	}
};