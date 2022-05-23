
#include "Game.h"
#include "TextureManager.h"
#include "Components.h"
#include "Map.h"
#include "Collision.h"
#include "RenderText.h"
#include "Menu.h"
// create 
Manager manager;
auto& player(manager.addEntity());
auto& player2(manager.addEntity());
Map* map;
std::string mapPath ;
Mix_Chunk* explode = NULL;


AmmoManager* ammoManager = new AmmoManager(); //testing
SDL_Renderer* Game::renderer = NULL;
SDL_Event Game::event;
//LTexture* ltexture = new LTexture();

//for reloading and healing purposes
std::vector<int> player1Functions;
std::vector<int> player2Functions;



Game::Game()
{}
Game::~Game()
{}



void Game::init(const char* title, int x, int y, int width, int height, bool fullscreen)
{
	Menu::inGame = true;
	bool flags = false;
	if (fullscreen) {
		flags = SDL_WINDOW_FULLSCREEN;
	}
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		std::cout << "SDL initialized! \n";

		window = Menu::window;
		if (window)
		{
			std::cout << "created window!\n";
		}

		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer) {
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			std::cout << "renderer initialized!\n";
		}
		isRunning = true;
		explode = Mix_LoadWAV("assets/explosion.wav");
		// ecs
		map = new Map("assets/terrain.png", 1, 32);// map scale:1, tile size: 32
		if(mapPath == "")mapPath = "assets/map.map";
		map->LoadMap(mapPath, 39, 23);

		player.addComponent<TransformComponent>(400, 300);
		player.addComponent<SpriteComponent>("assets/tank.png");
		player.addComponent<CollisionComponent>("player1");

		player.addComponent<ShootComponent>(); //default option

		player2.addComponent<TransformComponent>(1000, 400);
		player2.addComponent<SpriteComponent>("assets/tank2.png");
		player2.addComponent<CollisionComponent>("player2");
		player2.addComponent<ShootComponent>(); //default option
		ShootComponent* test1 = &player.getComponent<ShootComponent>();
		ShootComponent* test2 = &player2.getComponent<ShootComponent>();
		std::cout << "Test1 address: " << test1 << std::endl;
		std::cout << "Test2 address: " << test2 << std::endl;
		ammoManager->addTankShootComponent(test1, test2);
		player.addGroup(groupPlayers);
		player2.addGroup(groupPlayers);

	}
	else {
		isRunning = false;
	}
	if (TTF_Init() < 0) {
		std::cout << "Error initializing SDL_ttf: " << TTF_GetError() << std::endl;
	}
	//things to reload and healing AND ALLAH MODE
	player1Functions.push_back(clock());
	player1Functions.push_back(clock());
	player2Functions.push_back(clock());
	player2Functions.push_back(clock());
	player2Functions.push_back(clock());
	player1Functions.push_back(clock());

}

auto& tiles(manager.getGroup(Game::groupMap));// pass in all the tiles into this 
auto& players(manager.getGroup(Game::groupPlayers));
auto& colliders(manager.getGroup(Game::groupColliders));

//chuyen dong cua xe tang---------------------------------
enum {
	up,
	down,
	left,
	right

};
bool states1[] = { false, false , false, false };//tank 1
bool states2[] = { false, false, false ,false };//tank 2
bool states3[] = { false,false,false,false };//shoot, reload, healing, allahMode for player of tank 2 
bool states4[] = { false, false, false, false };//tank 1
bool replayStates[] = { false, false }; //normal replay and reborn replay 

clock_t prevTimeForShootingPurpose = clock();
clock_t prevTimeShootingPlayer2 = clock();

clock_t replayingStart ;
//for render scoreboard
bool needRerenderScoreBoard = false;

//for render text showing status
//for render text
SDL_Surface* text;
SDL_Surface* text2;
SDL_Texture* text_texture;
SDL_Texture* text_texture2;
SDL_Surface* surfaceTextPlayer1;
SDL_Surface* surfaceTextPlayer2;
SDL_Texture* textureTextPlayer1;
SDL_Texture* textureTextPlayer2;
SDL_Texture* backgroundScoreboard;
SDL_Texture* tankDeadNoti;
std::string textPlayer1;
std::string textPlayer2;
TTF_Font* font;
bool keepTextPlayer1 = false;
bool keepTextPlayer2 = false;
//white color
bool reseting = false;
//for lock keydown events
bool lockKeyDownPlayer1 = false;
bool lockKeyDownPlayer2 = false;
void Game::handleEvents()
{
	SDL_PollEvent(&event);


	switch (event.type)
	{
	case SDL_QUIT:
		isRunning = false;
		break;
	case SDL_KEYDOWN:
		//player 2
		if (!lockKeyDownPlayer2) {
			if (Game::event.key.keysym.sym == SDLK_LEFT) states2[left] = true;
			if (Game::event.key.keysym.sym == SDLK_RIGHT) states2[right] = true;
			if (Game::event.key.keysym.sym == SDLK_UP) states2[up] = true;
			if (Game::event.key.keysym.sym == SDLK_DOWN) states2[down] = true;
		}


		//player1
		if (!lockKeyDownPlayer1) {
			if (Game::event.key.keysym.sym == SDLK_a) states1[left] = true;
			if (Game::event.key.keysym.sym == SDLK_d) states1[right] = true;
			if (Game::event.key.keysym.sym == SDLK_w) states1[up] = true;
			if (Game::event.key.keysym.sym == SDLK_s) states1[down] = true;
			//for new shoot function
		//tank1 shoot function
			if (Game::event.key.keysym.sym == SDLK_SPACE) states4[0] = true;
			if (Game::event.key.keysym.sym == SDLK_r) states4[1] = true;
			if (Game::event.key.keysym.sym == SDLK_q)
			{
				if (Menu::healingOrNot == true)
				{
					states4[2] = true;
				}
			}
			if (Game::event.key.keysym.sym == SDLK_TAB) states4[3] = true;
		}

		if (!lockKeyDownPlayer2) {
			//tank2 shoot function 
			if (Game::event.key.keysym.sym == SDLK_KP_0) states3[0] = true;
			if (Game::event.key.keysym.sym == SDLK_KP_PERIOD) states3[1] = true;
			if (Game::event.key.keysym.sym == SDLK_KP_1)
			{
				if (Menu::healingOrNot == true)
				{
					states3[2] = true;
				}
			}
			if (Game::event.key.keysym.sym == SDLK_KP_2) states3[3] = true;
			
			//NOT YET DONE: TWO TANK CHEAT CODES 
		}
		if (!lockKeyDownPlayer1 || !lockKeyDownPlayer2) {
			if (ammoManager->tankIsDead) {
				if (Game::event.key.keysym.sym == SDLK_MINUS)replayStates[0] = true;
				if (Game::event.key.keysym.sym == SDLK_EQUALS)replayStates[1] = true;
			}
			
		}


		//access Menu
		if (Game::event.key.keysym.sym == SDLK_o)
		{
			Menu::handleOptionsEvent();
		}
		if (Game::event.key.keysym.sym == SDLK_ESCAPE)
		{
			Menu::handleMenuEvent();
		}
		if (Game::event.key.keysym.sym == SDLK_t)
		{
			Menu::handleTankSizeEvent();
		}
		if (Game::event.key.keysym.sym == SDLK_m)
		{
			Menu::checkAccessSoundFromGame = true;
			Menu::handleSoundEvent();
		}
		if (Game::event.key.keysym.sym == SDLK_b)
		{
			Menu::checkAccessShowBulletFromGame = true;
			Menu::handleShowBullet();
		}
		break;
	case SDL_KEYUP:

		if (!lockKeyDownPlayer1) {
			if (Game::event.key.keysym.sym == SDLK_a) states1[left] = false;
			if (Game::event.key.keysym.sym == SDLK_d) states1[right] = false;
			if (Game::event.key.keysym.sym == SDLK_w) states1[up] = false;
			if (Game::event.key.keysym.sym == SDLK_s) states1[down] = false;
			//for new shoot function
			if (Game::event.key.keysym.sym == SDLK_SPACE) states4[0] = false;
			if (Game::event.key.keysym.sym == SDLK_r) states4[1] = false;
			if (Game::event.key.keysym.sym == SDLK_q) states4[2] = false;
			if (Game::event.key.keysym.sym == SDLK_TAB) states4[3] = false;
		}
		if (!lockKeyDownPlayer2) {
			if (Game::event.key.keysym.sym == SDLK_LEFT) states2[left] = false;
			if (Game::event.key.keysym.sym == SDLK_RIGHT) states2[right] = false;
			if (Game::event.key.keysym.sym == SDLK_UP) states2[up] = false;
			if (Game::event.key.keysym.sym == SDLK_DOWN) states2[down] = false;
			// tank2 shoot function
			if (Game::event.key.keysym.sym == SDLK_KP_0) states3[0] = false;
			if (Game::event.key.keysym.sym == SDLK_KP_PERIOD) states3[1] = false;
			if (Game::event.key.keysym.sym == SDLK_KP_1) states3[2] = false;
			if (Game::event.key.keysym.sym == SDLK_KP_2) states3[3] = false;
			//NOT YET DONE: TWO TANK CHEAT CODES (FOR TESTING PURPOSES)
		}
		if (!lockKeyDownPlayer1 || !lockKeyDownPlayer2) {
			if (Game::event.key.keysym.sym == SDLK_n)replayStates[0] = false;
			if (Game::event.key.keysym.sym == SDLK_m)replayStates[1] = false;
		}

		break;
	}

}
//---------------------------------------------------------

void Game::update()
{

	if (Menu::needToChangeTankSize) {
		ammoManager->setSizeTank(Menu::newTankWidth, Menu::newTankHeight);
		Menu::needToChangeTankSize = false;
		std::cout << "Change tank size successful in ammoManager" << std::endl;
	}
	clock_t currentTimeForShootingPurpose = clock();
	clock_t currentTimeShootingPlayer2 = clock();
	SDL_Rect playerCol = player.getComponent<CollisionComponent>().collider;
	SDL_Rect player2Col = player2.getComponent<CollisionComponent>().collider;

	manager.refresh();
	manager.update();
	// neu co va cham

	for (auto& c : colliders)
	{
		SDL_Rect cCol = c->getComponent<CollisionComponent>().collider;
		if (Collision::AABB(playerCol, cCol))
		{
			player.getComponent<TransformComponent>().diThang(player.getComponent<TransformComponent>().speed * -2);

		}

		if (Collision::AABB(player2Col, cCol))
		{
			player2.getComponent<TransformComponent>().diThang(player2.getComponent<TransformComponent>().speed * -2);
		}
		//CHECK COLLISION OF PROJECTILES WITH COLLIDER tank 1
		//remember: Allah mode allow projectile to bypass colliders
		if (!player.getComponent<ShootComponent>().allahMode) {
			for (int iter = 0; iter < ammoManager->projectilesPlayer1.size(); iter++) {
				SDL_Rect tempToCheck;
				tempToCheck.x = ammoManager->projectilesPlayer1[iter].x;
				tempToCheck.y = ammoManager->projectilesPlayer1[iter].y;
				tempToCheck.w = 34;
				tempToCheck.h = 10;
				if (Collision::AABB(tempToCheck, cCol)) {
					std::cout << "Detect collision with collider. Position: " << tempToCheck.x << " " << tempToCheck.y << std::endl;
					ammoManager->projectilesPlayer1.erase(ammoManager->projectilesPlayer1.begin() + iter);
					ammoManager->projectilesAnglesPlayer1.erase(ammoManager->projectilesAnglesPlayer1.begin() + iter);
					Mix_PlayChannel(-1, explode, 0);
				}
			}
		}

		// CHECK COLLISION OF PROJECTILES WITH COLLIDER tank 2
		//remember: Allah mode allow projectile to bypass colliders
		if (!player2.getComponent<ShootComponent>().allahMode) {
			for (int iter = 0; iter < ammoManager->projectilesPlayer2.size(); iter++) {
				SDL_Rect tempToCheck;
				tempToCheck.x = ammoManager->projectilesPlayer2[iter].x;
				tempToCheck.y = ammoManager->projectilesPlayer2[iter].y;
				tempToCheck.w = 34;
				tempToCheck.h = 10;
				if (Collision::AABB(tempToCheck, cCol)) {
					std::cout << "Detect collision with collider. Position: " << tempToCheck.x << " " << tempToCheck.y << std::endl;
					ammoManager->projectilesPlayer2.erase(ammoManager->projectilesPlayer2.begin() + iter);
					ammoManager->projectilesAnglesPlayer2.erase(ammoManager->projectilesAnglesPlayer2.begin() + iter);
					Mix_PlayChannel(-1, explode, 0);
				}
			}
		}



	}

	// di chuyen
	if (states1[right]) player.getComponent<TransformComponent>().rePhai();
	if (states1[left]) player.getComponent<TransformComponent>().reTrai();
	if (states1[up])
	{
		player.getComponent<TransformComponent>().speed = 2;
		player.getComponent<TransformComponent>().diThang();
	}
	if (states1[down])
	{

		player.getComponent<TransformComponent>().speed = -2;
		player.getComponent<TransformComponent>().diThang();

	}
	if (states2[right]) player2.getComponent<TransformComponent>().rePhai();
	if (states2[left]) player2.getComponent<TransformComponent>().reTrai();
	if (states2[up])
	{

		player2.getComponent<TransformComponent>().speed = 2;
		player2.getComponent<TransformComponent>().diThang();

	}
	if (states2[down])
	{

		player2.getComponent<TransformComponent>().speed = -2;
		player2.getComponent<TransformComponent>().diThang();

	}
	//bo sung: Neu tank di tran man hinh
	//tank1 tran man hinh
	if (player.getComponent<TransformComponent>().position.x < 0 ||
		player.getComponent<TransformComponent>().position.x > 1248 - player.getComponent<TransformComponent>().width * player.getComponent<TransformComponent>().scale ||
		player.getComponent<TransformComponent>().position.y > 736 - player.getComponent<TransformComponent>().height * player.getComponent<TransformComponent>().scale ||
		player.getComponent<TransformComponent>().position.y < 0
		) {

		player.getComponent<TransformComponent>().vaChamManHinh();

	}

	//tank 2 tran man hinh
	if (player2.getComponent<TransformComponent>().position.x < 0 ||
		player2.getComponent<TransformComponent>().position.x > 1248 - player2.getComponent<TransformComponent>().width * player2.getComponent<TransformComponent>().scale ||
		player2.getComponent<TransformComponent>().position.y > 736 - player2.getComponent<TransformComponent>().height * player2.getComponent<TransformComponent>().scale ||
		player2.getComponent<TransformComponent>().position.y < 0
		) {

		player2.getComponent<TransformComponent>().vaChamManHinh();

	}
	//-----------het bo sung khi tank di tran man hinh

	//---------------------------------------------------------------------------- va cham	
	//When shooting
	//tank 1 shooting
	if (states4[0]) {
		if (currentTimeForShootingPurpose - prevTimeForShootingPurpose > player.getComponent<ShootComponent>().delayBetweenProjectiles
			&& player.getComponent<ShootComponent>().currentBullet > 0) {//HAVE TO MODIFIED
			ammoManager->tankShoot(player.getComponent<TransformComponent>().position, player.getComponent<TransformComponent>().angle, 1);
			prevTimeForShootingPurpose = currentTimeForShootingPurpose;
		}
	}
	//When reloading
	if (states4[1]) {
		if (!lockKeyDownPlayer1) {
			clock_t tempClock = clock();
			player1Functions[0] = tempClock;
			ammoManager->renderTextStatusPlayer1 = true;
		}
		lockKeyDownPlayer1 = true;
		clock_t compareClock = clock();
		states1[up] = false;
		states1[down] = false;
		states1[left] = false;
		states1[right] = false;
		//player.getComponent<TransformComponent>().dungDotNgot();
		if (compareClock - player1Functions[0] <= player.getComponent<ShootComponent>().delayTimeReload * 1000) {
			//std::cout << "Please waiting! Time 1" <<clock()<<" Time 2 "<<player1Functions[0]<< std::endl;
			states4[1] = true;
		}
		else {
			player.getComponent<ShootComponent>().reloading();
			ammoManager->needToRerenderScoreBoard_ = true;
			lockKeyDownPlayer1 = false;
			states4[1] = false;
			ammoManager->renderTextStatusPlayer1 = false;
		}

		//	std::cout << "Current bullet after reloading: " << player.getComponent<ShootComponent>().currentBullet << std::endl;
	}
	// When healing
	if (states4[2]) {
		if (!lockKeyDownPlayer1) {
			clock_t tempClock = clock();
			player1Functions[1] = tempClock;
			ammoManager->renderTextStatusPlayer1 = true;
		}
		lockKeyDownPlayer1 = true;
		clock_t compareClock = clock();
		states1[up] = false;
		states1[down] = false;
		states1[left] = false;
		states1[right] = false;
		//player.getComponent<TransformComponent>().dungDotNgot();
		if (compareClock - player1Functions[1] <= player.getComponent<ShootComponent>().delayTimeReload * 1000) {
			//std::cout << "Please waiting! Time 1" <<clock()<<" Time 2 "<<player1Functions[0]<< std::endl;
			states4[2] = true;
		}
		else {
			
			player.getComponent<ShootComponent>().healing();
			ammoManager->needToRerenderScoreBoard_ = true;
			lockKeyDownPlayer1 = false;
			states4[2] = false;
			ammoManager->renderTextStatusPlayer1 = false;
		}
		//	std::cout << "Current health after healing: " << player.getComponent<ShootComponent>().currentHealth << std::endl;
	}

	//When tank enable allahMode
	if (states4[3]) {
		player.getComponent<ShootComponent>().allahStyle();
		states4[3] = false;
		ammoManager->needToRerenderScoreBoard_ = true;
	}

	// Tank2 functions goes here
	if (states3[0]) {
		if (currentTimeShootingPlayer2 - prevTimeShootingPlayer2 >= player2.getComponent<ShootComponent>().delayBetweenProjectiles
			&& player2.getComponent<ShootComponent>().currentBullet > 0) {
			ammoManager->tankShoot(player2.getComponent<TransformComponent>().position, player2.getComponent<TransformComponent>().angle, 2);
			prevTimeShootingPlayer2 = currentTimeShootingPlayer2;
		}
	}
	if (states3[1]) {
		if (!lockKeyDownPlayer2) {
			clock_t tempClock = clock();
			player2Functions[0] = tempClock;
			ammoManager->renderTextStatusPlayer2 = true;
		}
		lockKeyDownPlayer2 = true;
		clock_t compareClock = clock();
		states2[up] = false;
		states2[down] = false;
		states2[left] = false;
		states2[right] = false;
		//player.getComponent<TransformComponent>().dungDotNgot();
		if (compareClock - player2Functions[0] <= player2.getComponent<ShootComponent>().delayTimeReload * 1000) {
			//std::cout << "Please waiting! Time 1" <<clock()<<" Time 2 "<<player1Functions[0]<< std::endl;
			states3[1] = true;
		}
		else {
			player2.getComponent<ShootComponent>().reloading();
			ammoManager->needToRerenderScoreBoard_ = true;
			lockKeyDownPlayer2 = false;
			states3[1] = false;
			ammoManager->renderTextStatusPlayer2 = false;
		}
		//	std::cout << "Current bullet after reloading: " << player2.getComponent<ShootComponent>().currentBullet << std::endl;
	}
	if (states3[2]) {
		if (!lockKeyDownPlayer2) {
			clock_t tempClock = clock();
			player2Functions[1] = tempClock;
			ammoManager->renderTextStatusPlayer2 = true;
		}
		lockKeyDownPlayer2 = true;
		clock_t compareClock = clock();
		states2[up] = false;
		states2[down] = false;
		states2[left] = false;
		states2[right] = false;
		//player.getComponent<TransformComponent>().dungDotNgot();
		if (compareClock - player2Functions[1] <= player2.getComponent<ShootComponent>().delayTimeReload * 1000) {
			//std::cout << "Please waiting! Time 1" <<clock()<<" Time 2 "<<player1Functions[0]<< std::endl;
			states3[2] = true;
		}
		else {
			player2.getComponent<ShootComponent>().healing();
			ammoManager->needToRerenderScoreBoard_ = true;
			lockKeyDownPlayer2 = false;
			states3[2] = false;
			ammoManager->renderTextStatusPlayer2 = false;
		}
		//	std::cout << "Current health after healing: " << player2.getComponent<ShootComponent>().currentHealth << std::endl;
	}
	if (states3[3]) {
		player2.getComponent<ShootComponent>().allahStyle();
		states4[3] = false;
		ammoManager->needToRerenderScoreBoard_ = true;
	}
	//----------end added functions
	//when dead and want to replay
	if (replayStates[0]) {
		player.getComponent<ShootComponent>().resetTankStatus(1);
		player2.getComponent<ShootComponent>().resetTankStatus(1);
		ammoManager->tankDead = 0;
		ammoManager->tankIsDead = false;
		reseting = true;
		replayStates[0] = false;
		replayingStart = clock();
		std::cout << "replaying..." << std::endl;
	}
	else if (replayStates[1]) {
		if (ammoManager->tankDead == 1) {
			player.getComponent<ShootComponent>().resetTankStatus(2);
			player2.getComponent<ShootComponent>().resetTankStatus(1);
		}
		else {
			player.getComponent<ShootComponent>().resetTankStatus(1);
			player2.getComponent<ShootComponent>().resetTankStatus(2);
		}
		ammoManager->tankDead = 0;
		ammoManager->tankIsDead = false;
		reseting = true;
		replayStates[1] = false;
		replayingStart = clock();
		std::cout << "replaying..." << std::endl;
	}
	if (clock() - replayingStart > 3000 && reseting) {
		reseting = false;
		ammoManager->needToRerenderScoreBoard_ = true;
		player.getComponent<TransformComponent>().position.x = 90;
		player.getComponent<TransformComponent>().position.y = 90;
		states1[up] = false;
		states1[down] = false;
		states1[left] = false;
		states1[right] = false;
		player2.getComponent<TransformComponent>().position.x = 1000;
		player2.getComponent<TransformComponent>().position.y = 90;
		states2[up] = false;
		states2[down] = false;
		states2[left] = false;
		states2[right] = false;
		if (player.getComponent<ShootComponent>().allahMode) {
			//return like before allah mode
			player.getComponent<ShootComponent>().damagePerShot /= 2;
			player.getComponent<ShootComponent>().delayTimeReload *= 2;
			player.getComponent<ShootComponent>().allahMode = false;
		}
		if (player2.getComponent<ShootComponent>().allahMode) {
			//return like before allah mode
			player2.getComponent<ShootComponent>().damagePerShot /= 2;
			player2.getComponent<ShootComponent>().delayTimeReload *= 2;
			player2.getComponent<ShootComponent>().allahMode = false;
		}
		//just to make sure that everything will be ok
		player.getComponent<ShootComponent>().resetTankStatus(1);
		player2.getComponent<ShootComponent>().resetTankStatus(1);
	}
	//THINGS HAVE TO CHECK EVERY FRAME
	//ADD TANK RECT
	ammoManager->addToSDLRect1(player.getComponent<TransformComponent>().position.x, player.getComponent<TransformComponent>().position.y);
	ammoManager->addToSDLRect2(player2.getComponent<TransformComponent>().position.x, player2.getComponent<TransformComponent>().position.y);
	//CHECK BULLET 
	ammoManager->checkBulletForPlayer1();
	ammoManager->checkBulletForPlayer2();
	//CHECK MINUSHEALTH OF PLAYER 1 IN ALLAH MODE
	if (player.getComponent<ShootComponent>().allahMode) {
		player.getComponent<ShootComponent>().autoMinusHealthOfAllahStyle();
		if (player.getComponent<ShootComponent>().needUpdateScoreBoard()) {
			ammoManager->needToRerenderScoreBoard_ = true;
		}

	}
	//CHECK MINUSHEALTH OF PLAYER 2 IN ALLAH MODE
	if (player2.getComponent<ShootComponent>().allahMode) {
		player2.getComponent<ShootComponent>().autoMinusHealthOfAllahStyle();
		if (player2.getComponent<ShootComponent>().needUpdateScoreBoard()) {
			ammoManager->needToRerenderScoreBoard_ = true;
		}
	}
	//check health of both tanks
	ammoManager->checkHealth();
	//New: What happen if tank out of screen already?
	if (player.getComponent<TransformComponent>().position.x < 0 ||
		player.getComponent<TransformComponent>().position.x > 1248 - player.getComponent<TransformComponent>().width * player.getComponent<TransformComponent>().scale ||
		player.getComponent<TransformComponent>().position.y > 736 - player.getComponent<TransformComponent>().height * player.getComponent<TransformComponent>().scale ||
		player.getComponent<TransformComponent>().position.y < 0
		) {
		player.getComponent<TransformComponent>().position.x = 90;
		player.getComponent<TransformComponent>().position.y = 90;
		states1[up] = false;
		states1[down] = false;
		states1[left] = false;
		states1[right] = false;
	}
	if (player2.getComponent<TransformComponent>().position.x < 0 ||
		player2.getComponent<TransformComponent>().position.x > 1248 - player2.getComponent<TransformComponent>().width * player2.getComponent<TransformComponent>().scale ||
		player2.getComponent<TransformComponent>().position.y > 736 - player2.getComponent<TransformComponent>().height * player2.getComponent<TransformComponent>().scale ||
		player2.getComponent<TransformComponent>().position.y < 0
		) {
		player2.getComponent<TransformComponent>().position.x = 1000;
		player2.getComponent<TransformComponent>().position.y = 90;
		states2[up] = false;
		states2[down] = false;
		states2[left] = false;
		states2[right] = false;
	}
}
void Game::render()
{
	SDL_RenderClear(renderer);
	//map->DrawMap();
	manager.draw();
	for (auto& t : tiles)
	{
		t->draw();
	}
	for (auto& c : colliders)
	{
		c->draw();
	}
	for (auto& p : players)
	{
		p->draw();
	}
	//Render projectiles player1
	if (Menu::checkShowBullet)
	{
		for (int i = 0; i < ammoManager->projectilesPlayer1.size(); i++) {
			SDL_Texture* loadProjectiles = TextureManager::LoadTexture("assets/ammo.png");
			std::cout << "Get load texture" << std::endl;
			SDL_Rect tempToRenderProjectile;
			/*double angle1 = player.getComponent<TransformComponent>().angle;
			double TANK_SIZE_WIDTH = player.getComponent<TransformComponent>().width;
			double TANK_SIZE_HEIGHT = player.getComponent<TransformComponent>().height;
			double offsetx = cos(0.01745329252 * (angle1)) * (TANK_SIZE_WIDTH / 2);
			double offsety = sin(0.01745329252 * (angle1)) * (TANK_SIZE_HEIGHT / 2);*/

			tempToRenderProjectile.x = ammoManager->projectilesPlayer1[i].x;
			tempToRenderProjectile.y = ammoManager->projectilesPlayer1[i].y ;
			std::cout << "Get SDL_REct x and y" << std::endl;
			tempToRenderProjectile.w = 34; //Projectiles size
			tempToRenderProjectile.h = 10; //Projectiles size
			SDL_Rect sourceRect;
			sourceRect.x = 0;
			sourceRect.y = 0;
			sourceRect.w = 89;
			sourceRect.h = 26;
			TextureManager::DrawTank(loadProjectiles, sourceRect, tempToRenderProjectile, ammoManager->projectilesAnglesPlayer1[i]);
			SDL_DestroyTexture(loadProjectiles);
		}
		//Render projectiles player2
		for (int i = 0; i < ammoManager->projectilesPlayer2.size(); i++) {
			SDL_Texture* loadProjectiles = TextureManager::LoadTexture("assets/ammo.png");
			std::cout << "Get load texture" << std::endl;
			SDL_Rect tempToRenderProjectile;
			/*double angle2 = player2.getComponent<TransformComponent>().angle;
			double TANK_SIZE_WIDTH = player2.getComponent<TransformComponent>().width;
			double TANK_SIZE_HEIGHT = player2.getComponent<TransformComponent>().height;
			double offsetx = cos(0.01745329252 * (angle2)) * (TANK_SIZE_WIDTH / 2);
			double offsety = sin(0.01745329252 * (angle2)) * (TANK_SIZE_HEIGHT / 2);*/

			tempToRenderProjectile.x = ammoManager->projectilesPlayer2[i].x;
			tempToRenderProjectile.y = ammoManager->projectilesPlayer2[i].y;
			std::cout << "Get SDL_REct x and y" << std::endl;
			tempToRenderProjectile.w = 34; //Projectiles size
			tempToRenderProjectile.h = 10; //Projectiles size
			SDL_Rect sourceRect;
			sourceRect.x = 0;
			sourceRect.y = 0;
			sourceRect.w = 89;
			sourceRect.h = 26;
			TextureManager::DrawTank(loadProjectiles, sourceRect, tempToRenderProjectile, ammoManager->projectilesAnglesPlayer2[i]);
			SDL_DestroyTexture(loadProjectiles);
		}
	}
	
	//bo comment phan duoi nay
	backgroundScoreboard = TextureManager::LoadTexture("assets/backgroundScoreboard.png");
	SDL_Rect sourceBS = { 0,0,1248,30 };
	SDL_Rect destBS = { 0,0,1248,30 };
	TextureManager::Draw(backgroundScoreboard, sourceBS, destBS);
	SDL_DestroyTexture(backgroundScoreboard);

	//SCOREBOARD
	if (ammoManager->needToRerenderScoreBoard()) {
		SDL_DestroyTexture(text_texture);
		SDL_FreeSurface(text);
		std::string scoreBoard = "Player1: Bullet: " + std::to_string(player.getComponent<ShootComponent>().currentBullet)
			+ " Health: " + std::to_string(player.getComponent<ShootComponent>().currentHealth);
		std::string scoreBoardPlayer2 = " Player2: Bullet: "
			+ std::to_string(player2.getComponent<ShootComponent>().currentBullet) + " Health: "
			+ std::to_string(player2.getComponent<ShootComponent>().currentHealth);
		if (player.getComponent<ShootComponent>().allahMode)scoreBoard += " Allah Mode Enabled";
		if (player2.getComponent<ShootComponent>().allahMode)scoreBoardPlayer2 += " Allah Mode Enabled";
		//initalize font
		font = TTF_OpenFont("assets/OpenSans-ExtraBold.ttf", 24);
		if (!font)std::cout << "Can't load font" << std::endl;
		SDL_Color color = { 255,255,255 };

		text = TTF_RenderText_Blended(font, scoreBoard.c_str(), color);
		if (!text)std::cout << "Can't load text" << std::endl;
		text_texture = SDL_CreateTextureFromSurface(renderer, text);
		SDL_Rect textDest = { 0,0,text->w,text->h };
		SDL_RenderCopy(renderer, text_texture, NULL, &textDest);
		//player2 part
		text2 = TTF_RenderText_Blended(font, scoreBoardPlayer2.c_str(), color);
		text_texture2 = SDL_CreateTextureFromSurface(renderer, text2);
		textDest = { 1248 / 2 ,0,text2->w,text2->h };
		SDL_RenderCopy(renderer, text_texture2, NULL, &textDest);
		std::cout << "Successful render scoreboard update of player 1." << std::endl;
		//------------end testing
	}
	else {
		SDL_Rect textDest = { 0,0,text->w,text->h };
		SDL_RenderCopy(renderer, text_texture, NULL, &textDest);
		textDest = { 1248 / 2,0,text2->w,text2->h };
		SDL_RenderCopy(renderer, text_texture2, NULL, &textDest);
	}
	if (ammoManager->needToRerenderTextStatusPlayer1()) {
		if (!keepTextPlayer1) {
			textPlayer1 = "Tank 1 is functioning. Please wait...";
			//initalize font

			font = TTF_OpenFont("assets/OpenSans-ExtraBold.ttf", 24);
			if (!font)std::cout << "Can't load font" << std::endl;
			SDL_Color color = { 255,255,255 };
			surfaceTextPlayer1 = TTF_RenderText_Blended(font, textPlayer1.c_str(), color);
			if (!surfaceTextPlayer1)std::cout << "Can't load text" << std::endl;
			textureTextPlayer1 = SDL_CreateTextureFromSurface(renderer, surfaceTextPlayer1);
			SDL_Rect testDest = { 0,700, surfaceTextPlayer1->w, surfaceTextPlayer1->h };
			SDL_RenderCopy(renderer, textureTextPlayer1, NULL, &testDest);
			keepTextPlayer1 = true;
			std::cout << "First time render success" << std::endl;

		}
		else {
			SDL_Rect testDest = { 0,700, surfaceTextPlayer1->w, surfaceTextPlayer1->h };
			SDL_RenderCopy(renderer, textureTextPlayer1, NULL, &testDest);
			std::cout << "NExt time render success" << std::endl;
		}

	}
	else {
		if (keepTextPlayer1) {
			SDL_DestroyTexture(textureTextPlayer1);
			SDL_FreeSurface(surfaceTextPlayer1);
		}
		keepTextPlayer1 = false;
	}


	if (ammoManager->needToRerenderTextStatusPlayer2()) {
		if (!keepTextPlayer2) {
			textPlayer2 = "Tank 2 is functioning. Please wait...";
			//initalize font
			font = TTF_OpenFont("assets/OpenSans-ExtraBold.ttf", 24);
			if (!font)std::cout << "Can't load font" << std::endl;
			SDL_Color color = { 255,255,255 };//white
			surfaceTextPlayer2 = TTF_RenderText_Blended(font, textPlayer2.c_str(), color);
			if (!surfaceTextPlayer2)std::cout << "Can't load text" << std::endl;
			textureTextPlayer2 = SDL_CreateTextureFromSurface(renderer, surfaceTextPlayer2);
			SDL_Rect testDest = { 1248 / 2,700, surfaceTextPlayer2->w, surfaceTextPlayer2->h };
			SDL_RenderCopy(renderer, textureTextPlayer2, NULL, &testDest);
			keepTextPlayer2 = true;

		}
		else {
			SDL_Rect testDest = { 1248 / 2,700, surfaceTextPlayer2->w, surfaceTextPlayer2->h };
			SDL_RenderCopy(renderer, textureTextPlayer2, NULL, &testDest);
		}
	}
	else {
		if (keepTextPlayer2) {
			SDL_DestroyTexture(textureTextPlayer2);
			SDL_FreeSurface(surfaceTextPlayer2);
		}
		keepTextPlayer2 = false;

	}
	//khi ammoManager->istankDead{
	if (ammoManager->tankIsDead) {
		if (ammoManager->tankDead == 1) {
			tankDeadNoti = TextureManager::LoadTexture("assets/player2win.png");
		}
		else {
			tankDeadNoti = TextureManager::LoadTexture("assets/player1win.png");
		}
		SDL_Rect sourceTDN = { 0,0,700,500 };
		SDL_Rect destTDN = { (1248 - 700) / 2,(736 - 500) / 2,700,500 };
		TextureManager::Draw(tankDeadNoti, sourceTDN, destTDN);
		SDL_DestroyTexture(tankDeadNoti);
	}
	if (reseting) {
		tankDeadNoti = TextureManager::LoadTexture("assets/reseting.png");
		SDL_Rect sourceTDN = { 0,0,1461,1044 };
		SDL_Rect destTDN = { (1248 - 700) / 2,(736 - 500) / 2,700,500 };
		TextureManager::Draw(tankDeadNoti, sourceTDN, destTDN);
		SDL_DestroyTexture(tankDeadNoti);
	}
//load anh


	SDL_RenderPresent(renderer);
	//SDL_DestroyTexture(text_texture);
}

void Game::close()
{
	Mix_FreeChunk(explode);
	explode = NULL;
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	renderer = NULL;
	window = NULL;
	Mix_CloseAudio();

	IMG_Quit();
	Mix_Quit();
	SDL_Quit();
	std::cout << "game closed!\n";
}