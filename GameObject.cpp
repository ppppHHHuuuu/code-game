
#include "GameObject.h"
#include "TextureManager.h"

GameObject::GameObject(const char* texturesheet, double x, double y)
{
	objTexture = TextureManager::LoadTexture(texturesheet);
	
	xpos = x;
	ypos = y;
}

void GameObject::Update()
{
	srcRect.h = 40;
	srcRect.w = 36;
	srcRect.x = 0;
	srcRect.y = 0;
	desRect.h = srcRect.h ;
	desRect.w = srcRect.w ;
	desRect.x = xpos;
	desRect.y = ypos;
	
}
void GameObject::rePhai()
{
	angle += 3;
	//if (angle < 0) angle = 360;
	std::cout << angle<<std::endl;
}

void GameObject::reTrai()
{
	angle -=3;
	//if (angle > 359) angle = 0;
	std::cout << angle<<std::endl;
}

void GameObject::diThang(double velocity) 
{
	double x_movement = cos(0.0174532925* (angle) ) * velocity;
	double y_movement = sin(0.0174532925*(angle)) * velocity;
	xpos += x_movement;
	ypos += y_movement;
}

void GameObject::Render()
{
	//SDL_Point* center;
	SDL_RenderCopyEx(Game::renderer,objTexture,&srcRect,&desRect,angle,NULL, SDL_FLIP_NONE);

}