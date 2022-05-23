#pragma once
#include "Game.h"

class GameObject {
public:
	GameObject(const char* texturesheet,double x, double y);
	~GameObject();
	void Update();
	void reTrai();
	void rePhai();
	void diThang(double velocity);
	void Render();

	double angle = 0;


//private:
	double xpos;
	double ypos;
	
	SDL_Texture* objTexture;
	SDL_Rect srcRect, desRect;


};
