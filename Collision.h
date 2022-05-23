#pragma once
#include <SDL.h>


class CollisionComponent;

class Collision
{
public:
	static bool AABB(const SDL_Rect& rectA, const SDL_Rect& rectB);// check collision theo cac truc
	static bool AABB(const CollisionComponent& colA, const CollisionComponent& colB);

};