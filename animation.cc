#include "animation.h"
//Se crea la animación
//Parametros: Textura, posición inicial, tamaño de los cuadros, número de cuadros en la animación, velocidad del recorrido de los cuadros
Animation::Animation(Texture &Tex, Vector2i Pos, Vector2i Size, int Count, float Speed)
{
	ActualFrame = 0;
	FrameSpeed = Speed;
	//Se añaden los distintos cuadros que tiene la textura para después ser recorridos y simular que la textura se mueve
	for (int i = 0; i < Count; i++)
	{
		Frames.push_back(IntRect(Pos.x + i * Size.x, Pos.y, Size.x, Size.y));
	}

	AnimSprite.setTexture(Tex);
	AnimSprite.setOrigin(Size.x / 2.f, Size.y / 2.f);
	AnimSprite.setTextureRect(Frames[0]);
}
//Se procesa el recorrido de la textura para simular que se mueve
void Animation::UpdateAnim()
{
	ActualFrame += FrameSpeed;
	int TotalFrames = (int)Frames.size();
	if (ActualFrame >= TotalFrames)
	{
		ActualFrame -= TotalFrames; //Reiniciar animación
	}
	if (TotalFrames > 0)
	{
		AnimSprite.setTextureRect(Frames[(int)ActualFrame]); //Establecer textura siguiente
	}
}

bool Animation::isEnd()
{
	if (ActualFrame + FrameSpeed >= Frames.size())
		return true;
	else
		return false;
}
