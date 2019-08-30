#include "spaceship.h"
//Se crea la nave, por defecto la propulsión está apagada
Spaceship::Spaceship()
{
	EntityID = "player";
	ThurstSoundOn = false;
	Propulsion = false;
}
//Se procesa el movimiento de la nave, así como la velocidad máxima del mismo y los límites de la ventana
void Spaceship::UpdateEntity()
{
	if (Propulsion)
	{
		EntityActualMoveXY.x += cos(EntityAngle*DEGTORAD)*0.2f;
		EntityActualMoveXY.y += sin(EntityAngle*DEGTORAD)*0.2f;
	}
	else
	{
		EntityActualMoveXY.x *= 0.99f;
		EntityActualMoveXY.y *= 0.99f;
	}
	int maxSpeed = 5;
	float Speed = (float)sqrt(double(EntityActualMoveXY.x*EntityActualMoveXY.x + EntityActualMoveXY.y*EntityActualMoveXY.y));

	if (Speed > maxSpeed)
	{
		EntityActualMoveXY *= maxSpeed / Speed;
	}

	EntityPos += EntityActualMoveXY;

	if (EntityPos.x > WindowSize.x) EntityPos.x = 0;
	if (EntityPos.x < 0) EntityPos.x = WindowSize.x;
	if (EntityPos.y > WindowSize.y) EntityPos.y = 0;
	if (EntityPos.y < 0) EntityPos.y = WindowSize.y;

}