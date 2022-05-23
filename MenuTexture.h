#pragma once

#include "Game.h"
#include "Menu.h"
#include <vector>

class MenuTexture
{
public:
	MenuTexture();
	~MenuTexture();

	enum TextColour
	{
		RED_TEXT,
		WHITE_TEXT,
		BLACK_TEXT,
	};
	void init(const char* title);
	//Loads image at specified path
	bool loadFromFile(std::string path);
	//Creates image from font string
	bool loadFromRenderedText(std::string textureText, SDL_Color textColor);
	// load texture
	SDL_Texture* loadTexture(std::string path);
	//Deallocates texture
	void free();
	//Set color modulation
	void SetColor(Uint8 red, Uint8 green, Uint8 blue);
	void SetColor(int type);// cac mau co ban
	SDL_Color GetColour();
	//Renders texture at given point
	int getWidth();
	int getHeight();
	void getDest(SDL_Rect& dest, int x, int y, int w, int h);
	void loadMedia(SDL_Event e);
	void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
	void Render(SDL_Event e);
	static void close();

	static SDL_Window* window;
	static TTF_Font* MenuFont;
	static SDL_Renderer* renderer;

private:
	//The actual hardware texture
	SDL_Texture* mTexture;
	SDL_Rect rect1;
	SDL_Rect rect2;
	//Image dimensions
	int mWidth;
	int mHeight;

	SDL_Color text_color;
};
extern MenuTexture textTexture[];
extern MenuTexture tankTexture[];
