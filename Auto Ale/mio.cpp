  /*struct PersonajeChapulin{
	int x;
	int y;
	int vidas;
	float salud;

	PersonajeChapulin(){

	}

	PersonajeChapulin(int _x, int _y, int _vidas, float _salud){
		x = _x;
		y = _y;
		vidas = _vidas;
		salud = _salud;
	}

};


  struct BalaChapulin{
	  int x;
	  int y;
	  int orientacion;
	  bool activado;

	  BalaChapulin(){

	  }

	  BalaChapulin(int _x, int _y, int _orientacion){
		  x = _x;
		  y = _y;
		  orientacion = _orientacion;
		  activado = false;
	  }

  };


  struct EnemigoBatman{
	  int x;
	  int y;
	  int tiempo;
	  float dano;
	  EnemigoBatman(){

	  }

	  EnemigoBatman(int _x, int _y, int _tiempo, float _dano){
		  x = _x;
		  y = _y;
		  tiempo = _tiempo;
		  dano = _dano;


	  }
  };


  struct BalaBatman{
	  int x;
	  int y;
	  float dano;

	  BalaBatman(){

	  }

	  BalaBatman(int _x, int _y, float _dano){
		  x = _x;
		  y = _y;
		  dano = _dano;
	  }
  };

  struct EnemigoSuperman{
	  int x;
	  int y;
	  int tiempo;
	  float dano;

	  EnemigoSuperman(){

	  }

	  EnemigoSuperman(int _x, int _y, int _tiempo, float _dano){
		  x = _x;
		  y = _y;
		  tiempo = _tiempo;
		  dano = _dano;


	  }
  };


  struct RayoSuperman{
	  int x;
	  int y;
	  int Tiempo;
	  float danno;

	  RayoSuperman(){

	  }

	  RayoSuperman(int _x, int _y, float _danno, int _Tiempo){
		  x = _x;
		  y = _y;
		  Tiempo = _Tiempo;
		  danno = _danno;
	  }
  };

  struct Bonus{
	  int x;
	  int y;
	  int tipo;
	  float salud;

	  Bonus(){

	  }

	  Bonus(int _x, int _y, int _tipo, int _salud){
		  x = _x;
		  y = _y;
		  tipo = _tipo;
		  salud = _salud;
	  }
  };


  void reproducirMusicaJuego();
  void reproducirDisparoChapulin();
  void reproducirDisparoSuperman();
  void reproducirDisparoBatman();
  void reproducirColisionBatmna();

  void dibujarMenu();
  void dibujarPuntaje();
  void dibujarSalud();
  void dibujarBarraSalud();
  void dibujarVidas();
  void dibujarBonus();
  void dibujarPrincipal(int, int, int);
  void dibujarEnemigoSuperman();
  void dibujarEnemigoBatman();
  void dibujarRayoEnemigoSuperman();
  void dibujarBalasChapulin();
  void dibujarMejoresPuntajes();
  void limpiarPantalla();

  void limpiarPuntajes();

  void guardarPuntajes();
  void cargarPuntajes();

  void sumarPuntaje(int);
  void sumarSalud(float);
  void sumarVida(int);

  void restarSalud(float);
  void restarVidas();
  void restaurarSalud();

  bool sinSalud();
  bool sinVidas();

  void generarEnemigoSuperman();
  void generarEnemigoBatman();
  void generarBonus();

  void crearRayoEnemigoSuperman(int,int);

  void desactivarBalaChapulin(int);
  void desactivarEnemigoSuperman(int);
  void desactivarEnemigoBatman(int);
  void desactivarBonus(int);
  void desactivarBalaEnemigoSuperman(int);

  void dispararPersonajeChapulin(int);
  void dispararEnemigoSuperman();

  void moverEnemigoBatman(int, int);
  void moverEnemigoSuperman(int, int);
  void moverPersonajeChapulin(int);
  void moverBalaPersonajeChapulin(int);
  void moverBalaEnemigoSuperman(int);

  void colisionSuperman();
  void colisionBatman();
  void colisionBalaEnemigoSuperman();
  void colisionBonus();

  void iniciarPuntajes();
  void iniciarPersonaje();
  void iniciarEnemigosSuperman();
  void iniciarEnemigosBatman();
  void iniciarBalasPersonajeChapulim();
  void iniciarBalasEnemigoSuperman();
  void iniciarBonus();


  void reproducirMusicaJuego(){
	  al_play_sample(musicaJuego, 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
  }

  void reproducirDisparoPersonajeChapulin(){
	  instanciaSonido = al_create_sample_instance(disparoPersonajeChapulin);
	  al_set_sample_instance_playmode(instanciaSonido, ALLEGRO_PLAYMODE_ONCE);
	  al_attach_sample_instance_to_mixer(instanciaSonido, al_get_default_mixer());
	  al_play_sample_instance(instanciaSonido);
  }

  void reproducirDisparoEnemigoSuperman(){
	  instanciaSonido = al_create_sample_instance(disparoEnemigoSuperman);
	  al_set_sample_instance_playmode(instanciaSonido, ALLEGRO_PLAYMODE_ONCE);
	  al_attach_sample_instance_to_mixer(instanciaSonido, al_get_default_mixer());
	  al_play_sample_instance(instanciaSonido);
  }

  void reproducirColisionEnemigoBatman(){
	  instanciaSonido = al_create_sample_instance(colisionEnemigoBatman);
	  al_set_sample_instance_playmode(instanciaSonido, ALLEGRO_PLAYMODE_ONCE);
	  al_attach_sample_instance_to_mixer(instanciaSonido, al_get_default_mixer());
	  al_play_sample_instance(instanciaSonido);

  }

  void limpiarPantalla(){
	  al_clear_to_color(transparente);
  }

  void limpiarPuntajes(){
	  for (int i = 0; i < mejoresPuntajesJuego; i++){
		  mejoresPuntajes[i] = 0;
	  }
  }

  void guardarPuntajes(){
	  documento = fopen("Puntajes.txt", "a");
	  fprintf(documento, "%i \n", puntaje);
	  fclose(documento);

  }

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

  void restarSalud(float danno){
	  personajeChapulin->salud -= danno;
  }

  void restarVidas(){
	  personajeChapulin->vidas -= 1;
  }

  void restaurarSalud(){
	  personajeChapulin->salud = 100.0;
  }

  bool sinSalud(){
	  if (personajeChapulin->salud <= 0.0) return true;

	  else return false;
  }
  bool sinVidas(){
	  if (personajeChapulin->vidas == 0) return true;
	  else return false;
  }
  */




 


