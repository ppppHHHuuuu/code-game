#include "MenuTexture.h"
const int SCREEN_WIDTH = 200;
const int SCREEN_HEIGHT = 150;

const int TANK_SIZE_HEIGHT = 36;
const int TANK_SIZE_WIDTH = 40;
enum TankSize
{
	small,
	medium,
	large
};
enum TextColour
{
	RED_TEXT,
	WHITE_TEXT,
	BLACK_TEXT,
};
SDL_Window* MenuTexture::window = NULL;
TTF_Font* MenuTexture::MenuFont = NULL;
SDL_Renderer* MenuTexture::renderer = NULL;


//SDL_Surface* MenuTexture::screenSurface = NULL;
//SDL_Surface* MenuTexture::PNGSurface = NULL;
MenuTexture textTexture[3];
MenuTexture tankTexture[2];

MenuTexture::MenuTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
	text_color.r = 255;
	text_color.b = 255;
	text_color.g = 255;
}

MenuTexture::~MenuTexture()
{
	//Deallocate
	free();
}

void MenuTexture::init(const char* title)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		std::cout << "SDL initialized! \n";

		window = SDL_CreateWindow("nofTankSize", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, false);
		if (!window)
		{
			std::cout << "create window for text failed" << std::endl;
		}
		if (TTF_Init() == -1)
		{
			printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
			//success = false;
		}
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer) {
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			std::cout << "renderer initialized!\n";
		}
	}
}

bool MenuTexture::loadFromFile(std::string path)
{
	//Get rid of preexisting texture
	/*free();*/

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Color key image
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

bool MenuTexture::loadFromRenderedText(std::string textureText, SDL_Color textColor)
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(MenuFont, textureText.c_str(), textColor);
	if (textSurface == NULL)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		//Create texture from surface pixels
		mTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
		if (mTexture == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}

	//Return success
	return mTexture != NULL;
}

void MenuTexture::free()
{
	//Free texture if it exists
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}
void MenuTexture::close()
{
	for (int i = 0; i < 3; i++)
	{
		textTexture[i].free();
	}
	for (int i = 0; i < 2; i++)
	{
		tankTexture[i].free();
	}
	TTF_CloseFont(MenuFont);
	MenuFont = NULL;
	SDL_DestroyRenderer(renderer);

	SDL_DestroyWindow(window);
	window = NULL;
	renderer = NULL;
}

void MenuTexture::SetColor(int type) {
	if (type == RED_TEXT) {
		SDL_Color color = { 255, 0, 0 };
		text_color = color;
	}
	else if (type == WHITE_TEXT) {
		SDL_Colour color = { 255, 255, 255 };
		text_color = color;
	}
	else if (type == BLACK_TEXT) {
		SDL_Colour color = { 0, 0, 0 };
		text_color = color;
	}

}
SDL_Color MenuTexture::GetColour()
{
	return	text_color;
}
void MenuTexture::SetColor(Uint8 red, Uint8 green, Uint8 blue) {
	text_color.r = red;
	text_color.b = blue;
	text_color.g = green;
}



void MenuTexture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	std::cout << "went to render func" << std::endl;
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };
	//Render to screen
	SDL_RenderCopyEx(renderer, mTexture, clip, &renderQuad, angle, center, flip);
}

int MenuTexture::getWidth()
{
	return mWidth;
}

int MenuTexture::getHeight()
{
	return mHeight;
}
SDL_Texture* MenuTexture::loadTexture(std::string path)
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
}

void MenuTexture::getDest(SDL_Rect& dest, int x, int y, int w, int h)
{
	dest.x = x;
	dest.y = y;
	dest.w = w;
	dest.h = h;
}
void MenuTexture::loadMedia(SDL_Event e)
{
	MenuFont = TTF_OpenFont("assets/font/Candarab.ttf", 20);
	if (MenuFont == NULL)
	{
		printf("Failed to load jetbrain font! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		textTexture[0].SetColor(BLACK_TEXT);
		textTexture[0].loadFromRenderedText("Change tanks size to", textTexture[0].GetColour());
		textTexture[1].loadFromRenderedText("20*18", textTexture[1].GetColour());
		textTexture[1].SetColor(RED_TEXT);
		if (e.key.keysym.sym == SDLK_1)
		{

			textTexture[1].loadFromRenderedText("20*18", textTexture[1].GetColour());

			getDest(rect1, 20, 40, 20, 18);
			getDest(rect2, 135, 40, 20, 18);

		}
		else if (e.key.keysym.sym == SDLK_2) {
			textTexture[1].loadFromRenderedText("40*36", textTexture[1].GetColour());

			getDest(rect1, 20, 40, 40, 36);
			getDest(rect2, 135, 40, 40, 36);
		}
		else if (e.key.keysym.sym == SDLK_3)
		{
			textTexture[1].loadFromRenderedText("80*72", textTexture[1].GetColour());

			getDest(rect1, 20, 40, 80, 72);
			getDest(rect2, 110, 40, 80, 72);
		}
	}
}


void MenuTexture::Render(SDL_Event e)
{
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(renderer);
	textTexture[0].render(0, 0);
	textTexture[1].render(75, 20);
	SDL_RenderCopy(renderer, tankTexture[0].loadTexture("assets/tank.png"), NULL, &rect1);
	SDL_RenderCopy(renderer, tankTexture[1].loadTexture("assets/tank2.png"), NULL, &rect2);
	SDL_RenderPresent(renderer);
	SDL_Delay(1000);
	SDL_DestroyWindow(window);
}
