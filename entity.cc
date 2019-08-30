#include "entity.h"
//Se crea la identidad, por defecto est� vivo para as� no ser eliminado al instante de haber sido creado
Entity::Entity()
{
	IsAlive = true;
}
//Se establecen las propiedades de la entidad
void Entity::SetEntity(Animation & anim, Vector2f pos, float angle, int radius, Vector2f windowsize)
{
	EntityAnim = anim;
	EntityPos = pos;
	EntityAngle = angle;
	EntityRadius = float(radius);
	WindowSize = windowsize;
}
//Se establece la posici�n y la rotaci�n de la identidad y se dibuja en la ventana de gr�ficos
void Entity::DrawEntity(RenderWindow & window)
{
	EntityAnim.AnimSprite.setPosition(EntityPos);
	EntityAnim.AnimSprite.setRotation(EntityAngle + 90);
	window.draw(EntityAnim.AnimSprite);
}
