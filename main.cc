#include <SFML/Graphics.hpp>
#include <time.h>
#include "game.h"

using namespace sf;
//Se crea el juego y se cambia la semilla del rand para obtener n�meros aleatorios (lo m�s aleatorio posible)
int main()
{
	srand((int)time(0));
	Game Asteroids({ 1280,800 }, "Asteroids");
	return 0;
}
