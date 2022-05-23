#pragma once
#include "Components.h"
#include <SDL.h>
#include "TextureManager.h"
#include "Menu.h"
class SpriteComponent : public Component
{
private:
	TransformComponent* transform;
	SDL_Texture* texture;
	SDL_Rect srcRect, desRect;

public:
	SpriteComponent();
	SpriteComponent(const char* path)
	{
		setTexture(path);
	}
	~SpriteComponent()
	{
		SDL_DestroyTexture(texture);
	}

	void setTexture(const char* path)
	{
		texture = TextureManager::LoadTexture(path);
	}

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>(); // tao 1 entity bang ecs
		srcRect.x = srcRect.y = 0;
		srcRect.w = transform->width;
		srcRect.h = transform->height;
	}

	void update() override
	{
		transform->scale = Menu::GetScale;
		desRect.x = transform->position.x;
		desRect.y = transform->position.y;
		desRect.w = transform->width * transform->scale;
		desRect.h = transform->height * transform->scale;
	}
	void draw() override
	{
		TextureManager::DrawTank(texture, srcRect, desRect, transform->angle);
	}
};