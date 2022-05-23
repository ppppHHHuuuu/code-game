
#include "Collision.h"
#include "CollisionComponent.h"

bool Collision::AABB(const SDL_Rect& rectA, const SDL_Rect& rectB)
{
	SDL_bool collide = SDL_HasIntersection(&rectA, &rectB);
	return collide;
}

bool Collision::AABB(const CollisionComponent& colA, const CollisionComponent& colB)
{
	if (AABB(colA.collider, colB.collider))
	{
		std::cout << colA.tag << " va cham " << colB.tag << std::endl;
		return true;
	}
	return false;
}
