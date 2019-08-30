#define PLAYER_COLISSION //Comentar en caso de querer jugar con vida infinita
#include "game.h"
//Abre una ventana de gráficos y manda a cargar los elementos del juego
Game::Game(Vector2i dim, std::string titulo) 
{
	Window = new RenderWindow(VideoMode(dim.x, dim.y), titulo.c_str());
	ConfigureGame();
}
//(1) Carga todos los recursos del juego
//(2) Inicia con 7 asteroides por defecto.
//(3) Crea el loop infinito que correrá el juego (cálculos, movimientos, gráficos, sonido, etc)
void Game::ConfigureGame()
{
	//(1)
	LoadGame();
	//(2)
	for (int i = 0; i < 8; i++)
	{
		Asteroid * tempAsteroid;
		tempAsteroid = new Asteroid;

		tempAsteroid->SetEntity(*RockAnim, { 0,float(rand() % Window->getSize().y) }, float(rand() % 360), 25, Vector2f(Window->getSize()));
		Entities.push_back(tempAsteroid);
	}
	//(3)
	while (Window->isOpen())
	{
		Event event;
		while (Window->pollEvent(event))
		{
			if (event.type == Event::Closed) Window->close();
			//Disparos
			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Space)
			{
				Laser * tempLaser;
				tempLaser = new Laser;
				tempLaser->SetEntity(*BulletAnim, PlayerSpaceship->EntityPos, (float)PlayerSpaceship->EntityAngle, 10, Vector2f(Window->getSize()));
				Entities.push_back(tempLaser);
				ShootSound->play();
			}
		}
		ProcessKeyboard();
		ProcessColissions();
		ProcessWindow();
	}
	
}
//Al perder, se reinician las variables y se eliminan todos los asteroides, seguidamente cargar 7 asteroides por defecto.
void Game::ResetGame()
{
	SShipExplosion->play();
	PlayerSpaceship->SetEntity(PlayerSpaceship->EntityAnim, Vector2f(Window->getSize()), -90, 20, Vector2f(Window->getSize()));
	PlayerSpaceship->EntityPos = { Window->getSize().x / 2.f,   Window->getSize().y / 2.f };
	PlayerSpaceship->EntityActualMoveXY = { 0,0 };

	for (auto z : Entities)
	{
		if (z->EntityID == "explosion" || z->EntityID == "laser" || z->EntityID == "asteroid")
		{
			z->IsAlive = 0;
		}
	}

	for (int i = 0; i < 8; i++)
	{
		Asteroid * tempAsteroid;
		tempAsteroid = new Asteroid;

		tempAsteroid->SetEntity(*RockAnim, { 0,float(rand() % Window->getSize().y) }, float(rand() % 360), 25, Vector2f(Window->getSize()));
		Entities.push_back(tempAsteroid);
	}

	LevelTimer->restart();
	RestartingGameClock = new Clock;
	isRestarting = true;
	playerScore = 0;
}
//Se procesan todas las teclas presionadas
void Game::ProcessKeyboard()
{
	if (Keyboard::isKeyPressed(Keyboard::Right)) PlayerSpaceship->EntityAngle += 8;
	if (Keyboard::isKeyPressed(Keyboard::Left)) PlayerSpaceship->EntityAngle -= 8;
	if (Keyboard::isKeyPressed(Keyboard::Up))
	{
		PlayerSpaceship->Propulsion = true;
		PlayerSpaceship->EntityAnim = *SpaceshipOnAnim;
		//Se cambia el enfoque de la textura para simular la activación del propulsor
		if (!PlayerSpaceship->ThurstSoundOn)
		{
			ThrustSound->play();
			ThrustSound->setLoop(true);
			PlayerSpaceship->ThurstSoundOn = true;
		}
	}
	else
	{
		//Se cambia el enfoque de la textura para simular la desactivación del propulsor
		PlayerSpaceship->Propulsion = false;
		PlayerSpaceship->EntityAnim = *SpaceshipAnim;
		ThrustSound->stop();
		PlayerSpaceship->ThurstSoundOn = false;
	}
}
//En cada ciclo del loop infinito, se limpia y dibuja en la ventana los gráficos correspondientes
void Game::ProcessWindow()
{
	Window->clear();
	Window->draw(*Background);

	stringstream intToString;
	intToString << playerScore;
	string finalString = intToString.str();
	GameFont[0]->setString(finalString);
	Window->draw(*GameFont[0]);
	GameFont[1]->setString(gameLevel);
	Window->draw(*GameFont[1]);

	ProcessAsteroids();
	for (auto i : Entities) i->DrawEntity(*Window);

	Window->display();
}
//Se procesan las muertes de los asteroides, así como el nivel del juego
void Game::ProcessAsteroids()
{
	//Procesar muerte de asteroide
	for (auto i = Entities.begin(); i != Entities.end();)
	{
		Entity *tempEntity = *i;
		tempEntity->UpdateEntity();
		tempEntity->EntityAnim.UpdateAnim();	

		if (tempEntity->IsAlive == false) 
		{ 
			i = Entities.erase(i);
			delete tempEntity; 
		}
		else i++;
	}
	for (auto z : Entities)
	{
		if (z->EntityID == "explosion")
		{
			if (z->EntityAnim.isEnd()) z->IsAlive = 0;
		}
		if (z->EntityID == "explosion2" && isRestarting == false)
		{
			if (z->EntityAnim.isEnd()) z->IsAlive = 0;
		}
	}
	//Determinar el nivel del juego
	if (rand() % ActualLevel() == 0)
	{
		Asteroid * tempAsteroid;
		tempAsteroid = new Asteroid;
		tempAsteroid->SetEntity(*RockAnim, { 0, float(rand() % Window->getSize().y) }, float(rand() % 360), 25, Vector2f(Window->getSize()));
		tempAsteroid->EntityID = "asteroid";
		Entities.push_back(tempAsteroid);
	}
	//Destruir la animación de la explosión de la nave después de 1.5 segundos de haber sido destruída
	if (isRestarting == true)
	{
		Time elapsedTime = RestartingGameClock->getElapsedTime();
		if (elapsedTime.asSeconds() > 1.5)
		{
			isRestarting = false;
			delete RestartingGameClock;
		}
	}
}
//Se procesan las colisiones de los asteroides con los laser y nave con los asteroides
void Game::ProcessColissions()
{
	for (auto A : Entities)
	{
		for (auto B : Entities)
		{
			if (A->EntityID == "asteroid" && B->EntityID == "laser")
			{
				if (isColliding(A, B))
				{
					A->IsAlive = false;
					B->IsAlive = false;

					Entity * tempEntity;
					tempEntity = new Entity;
					tempEntity->SetEntity(*ExplosionAnim, A->EntityPos, 0.0f, 1, Vector2f(Window->getSize()));
					tempEntity->EntityID = "explosion";
					Entities.push_back(tempEntity);
					AstExplosionSound->play();
					playerScore += 500;
					if (rand() % 3 == 0)
					{
						for (int i = 0;i < 2;i++)
						{
							if (A->EntityRadius == 15) continue;
							Entity * tempAsteroid;
							tempAsteroid = new Asteroid;
							tempAsteroid->SetEntity(*Rock2Anim, A->EntityPos, float(rand() % 360), 15, Vector2f(Window->getSize()));
							Entities.push_back(tempAsteroid);
						}
					}
				}
			}
			#ifdef PLAYER_COLISSION
			if (A->EntityID == "player" && B->EntityID == "asteroid")
			{
				if (isColliding(A, B))
				{
					B->IsAlive = false;
					Entity * tempEntity;
					tempEntity = new Entity;
					tempEntity->SetEntity(*Explosion2Anim, A->EntityPos, 0.0f, 0, Vector2f(Window->getSize()));
					tempEntity->EntityID = "explosion2";
					Entities.push_back(tempEntity);
					ResetGame();
				}
			}
			#endif
			
		}
	}
}
//Se verifica si dos entidades (ya sea asteroide, nave o laser) están lo suficientemente cerca para determinar que chocan
//*Idea del algoritmo obtenido de internet*
bool Game::isColliding(Entity * A, Entity * B)
{
	if ((B->EntityPos.x - A->EntityPos.x)*(B->EntityPos.x - A->EntityPos.x) + \
		(B->EntityPos.y - A->EntityPos.y)*(B->EntityPos.y - A->EntityPos.y) < \
		(A->EntityRadius + B->EntityRadius)*(A->EntityRadius + B->EntityRadius))
		return true;
	else
		return false;
}
//Devuelve el nivel actual del juego, entre más bajo, más difícil.
//Entre más bajo sea el número, mayor será la probabilidad de que los asteroides se generen.
int Game::ActualLevel()
{
	Time transcurredTime = LevelTimer->getElapsedTime();

	if (transcurredTime.asSeconds() < 15)
	{
		gameLevel = "LEVEL 1";
		GameFont[1]->setFillColor({0, 204, 51});
		return 100;
	}
	else if (transcurredTime.asSeconds() < 30)
	{
		gameLevel = "LEVEL 2";
		GameFont[1]->setFillColor({ 153, 204, 0 });
		return 75;
	}
	else if (transcurredTime.asSeconds() < 45)
	{
		gameLevel = "LEVEL 3";
		GameFont[1]->setFillColor({ 204, 153, 0 });
		return 50;
	}
	else if (transcurredTime.asSeconds() < 60)
	{
		gameLevel = "LEVEL 4";
		GameFont[1]->setFillColor({ 204, 51, 0 });
		return 25;
	}
	else
	{
		gameLevel = "LEVEL 5";
		GameFont[1]->setFillColor({ 204, 0, 0 });
		return 40;
	}
}
//(1) Inicializa variables, carga texturas, sonidos y fuentes de texto.
void Game::LoadGame()
{
	Window->setFramerateLimit(60);
	isRestarting = false;
	playerScore = 0;
	gameLevel = "LEVEL 1";
	LevelTimer = new Clock;

	GameFontF = new Font;
	GameFontF->loadFromFile("files/fonts/decoder.ttf");

	GameFont[0] = new Text;
	GameFont[0]->setFont(*GameFontF);
	GameFont[0]->setCharacterSize(40);
	GameFont[0]->setFillColor(Color::Green);
	GameFont[0]->setPosition(5, -15);

	GameFont[1] = new Text;
	GameFont[1]->setFont(*GameFontF);
	GameFont[1]->setCharacterSize(40);
	GameFont[1]->setFillColor(Color::Green);
	GameFont[1]->setPosition(float(Window->getSize().x) - 125, -15);

	stringstream intToString;
	intToString << playerScore;
	string finalString = intToString.str();
	GameFont[0]->setString(finalString);

	BackgroundT = new Texture;
	BackgroundT->loadFromFile("files/background.jpg");
	Background = new Sprite(*BackgroundT);
	Background->setScale(0.75, 0.75);

	BulletT = new Texture;
	BulletT->loadFromFile("files/fire_blue.png");
	BulletAnim = new Animation(*BulletT, { 0,0 }, { 32,64 }, 16, 0.8f);

	SpaceshipT = new Texture;
	SpaceshipT->loadFromFile("files/spaceship.png");
	SpaceshipAnim = new Animation(*SpaceshipT, { 40,0 }, { 40,40 }, 1, 0);
	SpaceshipOnAnim = new Animation(*SpaceshipT, { 40,40 }, { 40,40 }, 1, 0);

	ExplosionT1 = new Texture;
	ExplosionT1->loadFromFile("files/explosions/type_C.png");
	ExplosionAnim = new Animation(*ExplosionT1, { 0, 0 }, { 256, 256 }, 48, 0.5f);

	ExplosionT2 = new Texture;
	ExplosionT2->loadFromFile("files/explosions/type_B.png");
	Explosion2Anim = new Animation(*ExplosionT2, { 0, 0 }, { 192, 192 }, 64, 0.5f);

	RockT1 = new Texture;
	RockT1->loadFromFile("files/rock.png");
	RockAnim = new Animation(*RockT1, { 0,0 }, { 64,64 }, 16, 0.2f);
	RockAnim->AnimSprite.setPosition(400, 400);

	RockT2 = new Texture;
	RockT2->loadFromFile("files/rock_small.png");
	Rock2Anim = new Animation(*RockT2, { 0,0 }, { 64,64 }, 16, 0.2f);

	PlayerSpaceship = new Spaceship;
	PlayerSpaceship->SetEntity(*SpaceshipAnim, { Window->getSize().x / 2.f,   Window->getSize().y / 2.f }, -90, 20, Vector2f(Window->getSize()));
	Entities.push_back(PlayerSpaceship);

	ShootSound = new Sound;
	ShootSoundB = new SoundBuffer;
	ShootSoundB->loadFromFile("files/sounds/laser2.ogg");
	ShootSound->setBuffer(*ShootSoundB);
	ShootSound->setVolume(50);

	AstExplosionSound = new Sound;
	AstExplosionSoundB = new SoundBuffer;
	AstExplosionSoundB->loadFromFile("files/sounds/explosionasteroid.ogg");
	AstExplosionSound->setBuffer(*AstExplosionSoundB);
	AstExplosionSound->setVolume(50);

	ThrustSound = new Sound;
	ThrustSoundB = new SoundBuffer;
	ThrustSoundB->loadFromFile("files/sounds/thrust.ogg");
	ThrustSound->setBuffer(*ThrustSoundB);

	SShipExplosion = new Sound;
	SShipExplosionSoundB = new SoundBuffer;
	SShipExplosionSoundB->loadFromFile("files/sounds/explosionnave.ogg");
	SShipExplosion->setBuffer(*SShipExplosionSoundB);
}
