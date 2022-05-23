#pragma once

//#include "Game.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <SDL_mixer.h>
#include "Game.h"
#include "MenuTexture.h"
#include <string>
class Menu
{
public:
	Menu();
	~Menu();

	bool init(const char* title, int x, int y, int width, int height);

	static SDL_Surface* loadSurface(std::string path);

	static bool loadMenuMedia();
	static bool handleOptionsMedia();// load cac media cua option? lieu sound co can load media ko
	static bool loadSoundMedia();

	static void handleYN();
	static void handleOptionsEvent();
	static void handleSoundEvent();
	static void handleMenuEvent();
	static void handleTankSizeEvent();
	static void handleBloodEvent();
	static void handleShowBullet();
	static void chooseMap();
	static void renderMenu();
	static void ShowTextWindowWhileSelectingTank(SDL_Event event);
	void close();

	static double GetScale;
	static bool checkShowBullet;
	static bool checkAccessShowBulletFromGame;
	static bool checkAccessSoundFromGame;
	static SDL_Window* window;//???
	static Mix_Music* sound;
	static SDL_Renderer* renderer;
	static bool needToChangeTankSize;
	static int newTankWidth;
	static int newTankHeight;
	static bool healingOrNot;
	static bool inGame;

protected:
	static SDL_Event event;
	static SDL_Surface* PNGSurface;// source surface
	static SDL_Surface* screenSurface; // dest surface
};
extern bool exitMenu;
//extern bool showBullet;
