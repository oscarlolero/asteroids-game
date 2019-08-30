#ifndef _SPACESHIP_H_
#define _SPACESHIP_H_
#include "entity.h"
using namespace sf;
class Spaceship : public Entity
{
	public:
		Spaceship();
		void UpdateEntity();
		bool Propulsion;
		bool ThurstSoundOn;
	private:
};

#endif