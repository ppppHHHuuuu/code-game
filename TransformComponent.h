//TransformComponent la de xu ly cac di chuyen  vat the

#pragma once
#include "Components.h"
#include "Vector2D.h"
#include <cmath>
class TransformComponent : public Component
{
public:
	Vector2D position;
	Vector2D velocity;

	int height = 40;
	int width = 36;
	double scale = 1;
	double angle = 90;// don vi la do, dung de quay xe tang

	double speed = 4;

	bool blocked = false;

	TransformComponent()
	{
		position.Zero();
	}

	TransformComponent(int sc)
	{
		position.Zero();
		scale = sc;
	}

	TransformComponent(float x, float y)
	{
		position.x = x;
		position.y = y;
	}

	TransformComponent(float x, float y, int h, int w, int sc)
	{
		position.x = x;
		position.y = y;
		height = h;
		width = w;
		scale = sc;
	}

	void init() override
	{
		velocity.Zero();
	}
	void update() override
	{
		position.x += static_cast<int>(velocity.x * speed);
		position.y += static_cast<int>(velocity.y * speed);
	}

	void rePhai()
	{
		angle += 3;
		std::cout << angle<<std::endl;
		//if (angle < 0) angle = 360;
	}

	void reTrai()
	{
		angle -= 3;
		std::cout << angle << std::endl;
		//if (angle > 359) angle = 0;
	}

	void diThang()
	{
		double x_movement = cos(0.01745329252 * (angle)) * speed;
		double y_movement = sin(0.01745329252 * (angle)) * speed;
		position.x += x_movement;
		position.y += y_movement;
	}
	void diThang(int v)
	{
		double x_movement = cos(0.01745329252 * (angle)) * v;
		double y_movement = sin(0.01745329252 * (angle)) * v;
		position.x += x_movement;
		position.y += y_movement;
	}
	void vaChamManHinh() {
		double x_movement = cos(0.0174532925 * (angle)) * speed;
		double y_movement = sin(0.0174532925 * (angle)) * speed;
		position.x -= x_movement;
		position.y -= y_movement;
	}
	void dungDotNgot() {
		position.x = position.x;
		position.y = position.y;
		speed = 0.0;
	}
};