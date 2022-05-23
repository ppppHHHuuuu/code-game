#pragma once
#include <string>
#include <SDL.h>
#include "Components.h"
#include "TextureManager.h"

class CollisionComponent : public Component
{
public:
	SDL_Rect collider;
	std::string tag;

	SDL_Texture* tex;
	SDL_Rect srcR, destR;


	TransformComponent* transform;
	CollisionComponent(std::string t)
	{
		tag = t;
	}

	CollisionComponent(std::string t, int xpos, int ypos, int size)
	{
		tag = t;
		collider.x = xpos;
		collider.y = ypos;
		collider.h = collider.w = size;
	}

	void init() override
	{	// kiem tra xem da tao tranformcomponent chua, neu roi se co loi runtime
		if (!entity->hasComponent<TransformComponent>())
		{
			entity->addComponent<TransformComponent>();
		}
		transform = &entity->getComponent<TransformComponent>();
		
		//tex = TextureManager::LoadTexture("assets/coltex.png");
		srcR = { 0,0,32,32 };
		destR = { collider.x, collider.y, collider.w, collider.h };
		
	}

	void update() override
	{
		if (tag != "terrain")
		{
			collider.x = static_cast<int>(transform->position.x);
			collider.w = transform->width * transform->scale;
			collider.y = static_cast<int>(transform->position.y);
			collider.h = transform->height * transform->scale;
		}
		
		destR.x = collider.x;
		destR.y = collider.y;
		
		
	}

	void draw() override
	{
		//TextureManager::Draw(tex, srcR, destR);
	}
};