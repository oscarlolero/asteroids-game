#ifndef _ANIMATION_H_
#define _ANIMATION_H_
#include <SFML/Graphics.hpp>
using namespace sf;
using namespace std;
struct Animation
{
	public:
		Animation() {};
		Animation(Texture &Tex, Vector2i Pos, Vector2i Size, int Count, float Speed);
		void UpdateAnim();
		float ActualFrame, FrameSpeed;
		vector<IntRect> Frames;
		Sprite AnimSprite;
		bool isEnd();

	private:
};
#endif
