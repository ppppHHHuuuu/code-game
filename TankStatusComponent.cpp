
#include "TankStatusFunction.h"


ShootComponent::ShootComponent(int maxBullet, int currentBullet, float delayTimeReload, bool godMode
	, int maxHealth, int currentHealth, int damagePerShot, int amountPerHealing, int minusHealth, int player, int delayBetweennProjectiles) {
	this->maxBullet = maxBullet;
	this->currentBullet = currentBullet;
	this->delayTimeReload = delayTimeReload;
	this->godMode = godMode;
	this->maxHealth = maxHealth;
	this->currentHealth = currentHealth;
	this->damagePerShot = damagePerShot;
	this->amountPerHealing = amountPerHealing;
	this->minusHealth = minusHealth;
	this->projectiles.clear();
	this->player = player;
	this->angles.clear();
	this->delayBetweenProjectiles = delayBetweenProjectiles;
}

ShootComponent::ShootComponent(int health, int bullet, int strength, int player) {
	this->maxBullet = bullet;
	this->currentBullet = bullet;
	this->projectiles.clear();
	this->maxHealth = health;
	this->currentHealth = health;
	this->damagePerShot = strength;
	this->amountPerHealing = (int)(strength*3/2+1);
	this->minusHealth = strength*2;
	this->player = player;
	this->angles.clear();
}
ShootComponent::ShootComponent(bool godMode, int player) {
	this->godMode = true;
	this->maxBullet = 100;
	this->currentBullet = 100;
	this->projectiles.clear();
	this->player = player;
	this->maxHealth = 100;
	this->currentHealth = 100;
	this->damagePerShot = 100;
	this->amountPerHealing = 100;
	this->minusHealth = 0;
	this->angles.clear();
	
}
ShootComponent::~ShootComponent() {

}
// position AND direction OF AMMO MUST BE MANUALLY ADDED!
//right now animated of reloading and healing is the same to decrease the complexity.
//can be updated if we have enough time
//
void ShootComponent::shoot() {
	if (this->currentBullet > 0) {
		this->currentBullet--;
		projectileInit(positionProjectile, 25);
	}
}

void ShootComponent::delay(int number_of_seconds)
{
	// Converting time into milli_seconds
	int milli_seconds = 1000 * number_of_seconds;

	// Storing start time
	clock_t start_time = clock();

	// looping till required time is not achieved
	while (clock() < start_time + milli_seconds)
		;
}
void ShootComponent::reloading() {
	clock_t currentTime = clock();
	if (currentTime - prevReloading <= delayReloading)return;
	
	animatedReload = true;
	//notResponding = true;
	//ShootComponent::delay(delayTimeReload);
	
	currentBullet = maxBullet;
	animatedReload = false;
	prevReloading = currentTime + delayTimeReload * 1000;


}
void ShootComponent::healing() {
	clock_t currentTime = clock();
	if (currentTime - prevHealing <= delayHealing)return;
	animatedReload = true;
	//ShootComponent::delay(delayTimeReload);
	currentHealth += amountPerHealing;
	if (currentHealth > maxHealth)currentHealth = maxHealth;
	animatedReload = false;
	prevHealing = currentTime + delayTimeReload * 1000;
}

void ShootComponent::allahStyle()
{
	if (allahMode)return;
	allahMode = true;
	
	damagePerShot *= 2;
	delayTimeReload /= 2;
	std::cout << "Allah mode enabled" << std::endl;
	
}
void ShootComponent::addAmmoInformation(Vector2D position) {
	this->positionProjectile = position;
	//this->directionProjectile = direction;
}
void ShootComponent::addAngleOfThisShoot(double angle_) {
	this->angles.push_back(angle_);
}
void ShootComponent::autoMinusHealthOfAllahStyle() {
	clock_t currentTime = clock();
	if (currentTime - prevTime >= (delayTimeReload * 2 + 1) * 1000) {
		currentHealth -= minusHealth;
		std::cout << "Go go bro. Current health: " << currentHealth << std::endl;
		prevTime = currentTime;
		updateScoreBoard = true;
	}
	else {
		return;
	}
	
	//delay(delayTimeReload * 2 + 1);
}
Projectile::Projectile(Vector2D pos, double bulletSpeed) {
	this->x = pos.x;
	this->y = pos.y;
	//this->shootDirection = direction;
	this->bulletSpeed = bulletSpeed;
	this->frames = 0; //default
}
Projectile::Projectile(){}

void Projectile::update(double angle) {
	//this->x += shootDirection.x * bulletSpeed;
	//this->y += shootDirection.y * bulletSpeed;
	//???? function
	double x_movement = cos(0.01745329252 * (angle)) * bulletSpeed;
	double y_movement = sin(0.01745329252 * (angle)) * bulletSpeed;
	this->x += x_movement;
	this->y += y_movement;
	
	this->frames++;
}
void ShootComponent::projectileInit(Vector2D pos, double bulletSpeed) { 
	Projectile sample(pos, bulletSpeed);

	projectiles.push_back(sample);
	
	std::cout << "Projectile init successful" << std::endl;
	std::cout << "Size of projectiles: " << projectiles.size() << std::endl;
}
bool ShootComponent::needUpdateScoreBoard() {
	if (updateScoreBoard) {
		updateScoreBoard = false;
		return !updateScoreBoard;
	}
	else {
		return updateScoreBoard;
	}
}

void ShootComponent::resetTankStatus(int scale) {
	this->maxBullet*= scale;
	this->maxHealth*=scale;
	this->currentBullet = maxBullet;
	this->godMode = false;
	this->currentHealth = maxHealth;
	this->damagePerShot*=scale;
	this->amountPerHealing*=scale;
	this->projectiles.clear();
	this->angles.clear();
	//this->delayBetweenProjectiles = delayBetweenProjectiles;
}
//Add Ammo Information -> projectileInit (ALREADY IN TANKSTATUS COMPONENT) -> shoot function -> AmmoManager file.
