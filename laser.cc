#include "laser.h"
//Se crea la identidad laser, nombrando el mismo como "laser"
Laser::Laser()
{
	EntityID = "laser";
}
//Se procesa el movimiento del laser y en caso de salir de los límites de la ventana, su variable IsAlive se cambia a false para ser destruido
//en el siguiente loop del juego.
void Laser::UpdateEntity()
{
	EntityActualMoveXY.x = cos(EntityAngle*DEGTORAD)*6;
	EntityActualMoveXY.y = sin(EntityAngle*DEGTORAD)*6;

	EntityPos += EntityActualMoveXY;

	if (EntityPos.x > WindowSize.x || EntityPos.x < 0 \
		|| EntityPos.y > WindowSize.y || EntityPos.y < 0) IsAlive = false;
}
