
#include "TextureManager.h"

SDL_Texture* TextureManager::LoadTexture(const char* texture)
{
	SDL_Surface* tempSurface = IMG_Load(texture);
	SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
	SDL_FreeSurface(tempSurface);

	return tex;
}

void TextureManager::Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest)
{
	SDL_RenderCopy(Game::renderer, tex, &src, &dest);
}

void TextureManager::DrawTank(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, double angle)
{
	SDL_RenderCopyEx(Game::renderer, tex, &src, &dest, angle, NULL, SDL_FLIP_NONE);
}