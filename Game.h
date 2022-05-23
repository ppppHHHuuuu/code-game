#pragma once
#include <SDL.h>
#include <iostream>
#include <vector>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <string>
#include <cmath>
class CollisionComponent;

class Game
{
public:
	Game();
	~Game();

	void init(const char* title, int x, int y, int width, int height, bool fullscreen);
	void handleEvents();
	void update();
	void render();
	void close();
	static SDL_Renderer* renderer;
	static std::string mapPath;

	bool running() { return isRunning; };
	static SDL_Event event;

	bool isRunning;
	
	enum groupLabels : std::size_t
	{
		groupMap,
		groupPlayers,
		groupColliders
	};

private:

	SDL_Window* window;

};