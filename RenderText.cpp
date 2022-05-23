#include "RenderText.h"

LTexture::LTexture() {

}
void LTexture::renderScoreBoardPlayer1(SDL_Renderer* renderer, int currentBullet, int currentHealth) {
	std::string scoreBoard = "Player1: Bullet: " + std::to_string(currentBullet)
		+ " Health: " + std::to_string(currentHealth);
	this->font = TTF_OpenFont("assets/OpenSans-ExtraBold.ttf", 24);
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreBoard.c_str(), color);
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_Rect textDest = { 0,0,textSurface->w,textSurface->h };
	SDL_RenderCopy(renderer, textTexture, NULL, &textDest);
	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);
}

void LTexture::renderScoreBoardPlayer2(SDL_Renderer* renderer, int currentBullet, int currentHealth) {
	std::string scoreBoard = "Player2: Bullet: " + std::to_string(currentBullet)
		+ " Health: " + std::to_string(currentHealth);
	this->font = TTF_OpenFont("assets/OpenSans-ExtraBold.ttf", 24);
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreBoard.c_str(), color);
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_Rect textDest = { 1248/2,0,textSurface->w,textSurface->h };
	SDL_RenderCopy(renderer, textTexture, NULL, &textDest);
	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);
}