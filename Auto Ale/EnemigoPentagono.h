
struct EnemigoPentagono{
	int x;
	int y;
	int tiempo;
	int sprite;
	int sumarSprite;
	float danno;
	EnemigoPentagono(){

	}

	EnemigoPentagono(int _x, int _y, int _tiempo, int _sprite, float _danno){
		x = _x;
		y = _y;
		tiempo = _tiempo;
		sprite = _sprite;
		sumarSprite = 10;
		danno = _danno;


	}
};