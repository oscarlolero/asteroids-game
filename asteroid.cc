#include "asteroid.h"
#include "iostream"

//Se crea el asteroide, se establece hacia qué dirección se moverá el asteroide de manera aleatoria
Asteroid::Asteroid()
{
	EntityID = "asteroid";

	Vector2f MoveXY;
	MoveXY = { float(rand() % 8 - 4) , float(rand() % 8 - 4) };
	//Con este pequeño algoritmo se previene un bug que causaba que los asteoides se movieran de manera completamente
	//vertical pegado a la izquierda u horizontal pegado arriba
	while (MoveXY.x == 0 || MoveXY.y == 0)
	{
		MoveXY = { float(rand() % 8 - 4) , float(rand() % 8 - 4) };
	}
	EntityActualMoveXY = MoveXY;
	
}
//Actualizar las nuevas coordenadas del asteroide y establece límites de la ventana, igual que la nave
void Asteroid::UpdateEntity()
{
	EntityPos += EntityActualMoveXY;

	if (EntityPos.x > WindowSize.x)	EntityPos.x = 0;
	if (EntityPos.x < 0) EntityPos.x = WindowSize.x;
	if (EntityPos.y > WindowSize.y) EntityPos.y = 0;
	if (EntityPos.y < 0) EntityPos.y = WindowSize.y;
}

