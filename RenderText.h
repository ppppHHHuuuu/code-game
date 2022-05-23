#pragma once
#include<SDL_image.h>
#include<SDL_ttf.h>
#include<iostream>
#include<string>
#include<math.h>

const int SCREEN_WIDTH = 1248;
const int SCREEN_HEIGHT = 736;
//idea based on Lazyfoo; however, I trully understand what I'm typing.
class LTexture {
public:
	LTexture();
	TTF_Font* font = NULL;
	
	SDL_Window* gWindow = NULL;
	SDL_Renderer* gRenderer = NULL;
	std::string scoreBoardPlayer1;
	std::string scoreBoardPlayer2;
	SDL_Color color = { 255,255,255 };
	void renderScoreBoardPlayer1(SDL_Renderer* renderer, int currentBullet, int currentHealth);
	void renderScoreBoardPlayer2(SDL_Renderer* renderer, int currentBullet, int currentHealth);

};


