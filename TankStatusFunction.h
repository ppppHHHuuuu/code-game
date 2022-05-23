#pragma once
#include "Vector2D.h"
#include "Components.h"
#include <SDL.h>
#include "TextureManager.h"
#include <time.h>
#include <iostream>
#include "AmmoManager.h"

/*
File nay duoc tao de luu tru cac status cua tank 
*/

class Projectile {
public:
	int frames;
	float x, y;
	double bulletSpeed;
	//Vector2D shootDirection;
	Projectile(Vector2D pos, double bulletSpeed);
	Projectile();
	void update(double angle);


};

class ShootComponent : public Component {
public:
	int maxBullet = 10;
	int currentBullet = 10;
	float delayTimeReload = 2.0; //2seconds
	bool animatedReload = false; //to show something when reload
	bool godMode = false;
	int maxHealth = 10;
	int currentHealth = 10;
	int damagePerShot = 1;
	int amountPerHealing = 2;
	int minusHealth = 1;
	bool allahMode = false;
	Vector2D positionTank;
	//Vector2D directionProjectile;
	Vector2D positionProjectile;
	int player = 1;
	std::vector<Projectile> projectiles;
	std::vector<double>angles;
	clock_t prevTime = clock();
	int delayBetweenProjectiles = 500; //by miliseconds
	int delayHealing = 200;
	int delayReloading = 200;
	clock_t prevHealing = clock();
	clock_t prevReloading = clock();
	bool updateScoreBoard = false;
	//new variable for new mode in option in UI
	bool ableToHealing = true;
	bool showProjectiles = true;
	double scaleTank = 1.0;
	//default option
	ShootComponent() {

	}
	//full option
	ShootComponent(int maxBullet, int currentBullet, float delayTimeReload, bool godMode
		, int maxHealth, int currentHealth, int damagePerShot, int amountPerHealing, int minusHealth, int player, int delayBetweenProjectiles);
	//simple option (RECOMMENDED)
	ShootComponent(int health, int bullet, int strength, int player);
	//god option
	ShootComponent(bool godMode,  int player);
	void shoot();
	void delay(int number_of_seconds);
	void reloading();
	void projectileInit(Vector2D pos, double bulletSpeed);
	void healing();
	//void allahStyle
	void allahStyle();
	void addAmmoInformation(Vector2D position);
	void addAngleOfThisShoot(double angle_);
	void autoMinusHealthOfAllahStyle();
	bool needUpdateScoreBoard();
	void resetTankStatus(int scale);
	~ShootComponent();
	

};

