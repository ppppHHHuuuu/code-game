#pragma once
#include <SDL_image.h>
#include "Game.h"
class TextureManager {

public:
	static SDL_Texture* LoadTexture(const char* fileName);
	static void DrawTank(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, double angle);
	static void Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest);
};