#ifndef _GAME_H_
#define _GAME_H_
#include <list>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "spaceship.h"
#include "animation.h"
#include "entity.h"
#include "asteroid.h"
#include "laser.h"

using namespace sf;
using namespace std;
class Game 
{
	public:
		Game(Vector2i dim, std::string titulo);
	private:
		void ProcessKeyboard();
		void ProcessWindow();
		void ProcessAsteroids();
		void ProcessColissions();
		void ConfigureGame();
		void LoadGame();
		void ResetGame();
		int ActualLevel();
		bool isColliding(Entity *A, Entity *B);

		float DEGTORAD = 0.017453f;
		bool isRestarting;
		int playerScore;
		string gameLevel;
		RenderWindow * Window;
		Spaceship * PlayerSpaceship;
		Sprite * Background;
		Clock * RestartingGameClock;
		Clock * LevelTimer;
		Font * GameFontF;
		Text * GameFont[2];
		list<Entity*> Entities;

		Animation * RockAnim;
		Animation * Rock2Anim;
		Animation * BulletAnim;
		Animation * ExplosionAnim;
		Animation * Explosion2Anim;
		Animation * SpaceshipOnAnim;
		Animation * SpaceshipAnim;

		Texture * BackgroundT;
		Texture * SpaceshipT;
		Texture * BulletT;
		Texture * ExplosionT1;
		Texture * ExplosionT2;
		Texture * RockT1;
		Texture * RockT2;

		SoundBuffer * ShootSoundB;
		SoundBuffer * AstExplosionSoundB;
		SoundBuffer * ThrustSoundB;
		SoundBuffer * SShipExplosionSoundB;

		Sound * SShipExplosion;
		Sound * ThrustSound;
		Sound * ShootSound;
		Sound * AstExplosionSound;
};
#endif

