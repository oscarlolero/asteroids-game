#ifndef _ENTITY_H_
#define _ENTITY_H_

#include <SFML/Graphics.hpp>
#include "animation.h"
using namespace sf;
using namespace std;

class Entity
{
public:
	Entity();
	void SetEntity(Animation &anim, Vector2f pos, float angle, int radius, Vector2f windowsize);
	void DrawEntity(RenderWindow &window);

	virtual void UpdateEntity() {}

	Vector2f EntityPos, EntityActualMoveXY, WindowSize;
	float EntityRadius, EntityAngle;
	bool IsAlive;
	string EntityID;
	Animation EntityAnim;
	float DEGTORAD = 0.017453f;

	private:
		

};
#endif
