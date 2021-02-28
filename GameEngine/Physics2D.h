#pragma once

#include "Hitbox2D.h"

class Physics2D 
{
public:
	struct CollisionData
	{
		CollisionData(int hitDirection, Vector2 hitVec);

		int direction = COLLISION_NONE;
		float overlap = 0;
		Vector2 hitVector = Vector2();
	};

	static Physics2D* getPhysics2D();

	static const int COLLISION_NONE = 0;
	static const int COLLISION_DOWN = 1;
	static const int COLLISION_UP = 2;
	static const int COLLISION_LEFT = 5;
	static const int COLLISION_RIGHT = 10;

	static const int COLLISION_UP_LEFT = COLLISION_UP + COLLISION_LEFT; // 7
	static const int COLLISION_UP_RIGHT = COLLISION_UP + COLLISION_RIGHT; // 12
	static const int COLLISION_DOWN_LEFT = COLLISION_DOWN + COLLISION_LEFT; // 6
	static const int COLLISION_DOWN_RIGHT = COLLISION_DOWN + COLLISION_RIGHT; // 11
	static const int COLLISION_ALL = COLLISION_DOWN + COLLISION_UP + COLLISION_LEFT + COLLISION_RIGHT; // 

	CollisionData colliding(Hitbox2D hb1, Hitbox2D hb2);
	int hitDirection(Hitbox2D hb1, Hitbox2D hb2);

	static bool isCollisionDown(CollisionData collisionProperties);
	static bool isCollisionUp(CollisionData collisionProperties);
	static bool isCollisionLeft(CollisionData collisionProperties);
	static bool isCollisionRight(CollisionData collisionProperties);

	static bool isCollisionDown(int collisionProperties);
	static bool isCollisionUp(int collisionProperties);
	static bool isCollisionLeft(int collisionProperties);
	static bool isCollisionRight(int collisionProperties);

public:
	static inline Physics2D* physics2D = nullptr; // DO NOT USE
};