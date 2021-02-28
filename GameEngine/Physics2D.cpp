#include <cmath>
#include "Log.h"
#include "Physics2D.h"

#define DEFAULT_COLLISION_DIST Vector2()

Physics2D* Physics2D::getPhysics2D()
{
	if (physics2D == nullptr) {
		physics2D = new Physics2D();
	}
	return physics2D;
}

Physics2D::CollisionData Physics2D::colliding(Hitbox2D hb1, Hitbox2D hb2)
{
	if (hb1.type == Hitbox2D::TYPE_UNDEFINED || hb2.type == Hitbox2D::TYPE_UNDEFINED)
		return CollisionData(COLLISION_NONE, DEFAULT_COLLISION_DIST);

	if (hb1.type == Hitbox2D::TYPE_CIRCLE && hb2.type == Hitbox2D::TYPE_CIRCLE) {
		Vector2 posDifference = Vector2::subtract(hb1.circleCentre(), hb2.circleCentre());
		float combinedRadii = hb1.circleDiameter() + hb2.circleDiameter();
		float distance = Vector::qSqrt(posDifference.x() * posDifference.x() + posDifference.y() * posDifference.y());
		if (distance <= combinedRadii) {

			float r = combinedRadii - distance;
			float theta = std::atan(posDifference.y() / posDifference.x());
			float p = std::sin(theta) * r;
			float q = Vector::qSqrt(r * r - p * p);

			int hitDirec = hitDirection(hb1, hb2);

			if (isCollisionLeft(hitDirec)) {
				p = -p;
				q = -q;
			}
			float pErrCorrect = 0;
			return CollisionData(hitDirec, Vector2(q < 0 ? q - pErrCorrect : q + pErrCorrect, p < 0 ? p - pErrCorrect : p + pErrCorrect));
		}
		return CollisionData(COLLISION_NONE, DEFAULT_COLLISION_DIST);
	}
	if (hb1.type == Hitbox2D::TYPE_RECTANGLE && hb2.type == Hitbox2D::TYPE_RECTANGLE) {
		if (hb1.position.x() <= hb2.position.x() + hb2.rectSize().x() &&
			hb1.position.x() + hb1.rectSize().x() >= hb2.position.x() &&
			hb1.position.y() <= hb2.position.y() + hb2.rectSize().y() &&
			hb1.position.y() + hb1.rectSize().y() >= hb2.position.y()) {
			Vector2 hitVector = Vector2();

			int hitDirec = hitDirection(hb1, hb2);
			return CollisionData(hitDirec, hitVector);
		}

		return CollisionData(COLLISION_NONE, DEFAULT_COLLISION_DIST);
	}
	//if ((hb1.type == Hitbox2D::TYPE_CIRCLE && hb2.type == Hitbox2D::TYPE_RECTANGLE) || (hb1.type == Hitbox2D::TYPE_RECTANGLE && hb2.type == Hitbox2D::TYPE_CIRCLE)) {
	//	bool swap = false;
	//	if (hb2.type == Hitbox2D::TYPE_CIRCLE) {
	//		Hitbox2D circle = hb2;
	//		Hitbox2D rect = hb1;
	//		hb1 = circle;
	//		hb2 = rect;
	//		swap = true;
	//	}
	//	float testX = hb1.circleCentre().x();
	//	float testY = hb1.circleCentre().y();

	//	if (hb1.circleCentre().x() < hb2.rectPos().x()) {
	//		testX = hb2.rectPos().x();
	//	}
	//	else if (hb1.circleCentre().x() > hb2.rectPos().x() + hb2.rectSize().x()) {
	//		testX = hb2.rectPos().x() + hb2.rectSize().x();
	//	}

	//	if (hb1.circleCentre().y() < hb2.rectPos().y()) {
	//		testY = hb2.rectPos().y();
	//	}
	//	else if (hb1.circleCentre().y() > hb2.rectPos().y() + hb2.rectSize().y()) {
	//		testY = hb2.rectPos().y() + hb2.rectSize().y();
	//	}

	//	float distX = hb1.circleCentre().x() - testX;
	//	float distY = hb1.circleCentre().y() - testY;
	//	float distance = 1.f / Vector::qInvSqrt((distX * distX) + (distY * distY));

	//	if (distance <= hb1.circleDiameter()) {
	//		if (swap)
	//			return hitDirection(hb2, hb1);
	//		return hitDirection(hb1, hb2);
	//	}
	//	return CollisionData(COLLISION_NONE, DEFAULT_COLLISION_DIST);
	//}
	return CollisionData(COLLISION_NONE, DEFAULT_COLLISION_DIST);
}

int Physics2D::hitDirection(Hitbox2D hb1, Hitbox2D hb2)
{
	float hb1Right = hb1.type == Hitbox2D::TYPE_CIRCLE ? hb1.circleCentre().x() : (hb1.rectPos().x() + hb1.rectSize().x());
	float hb1Left = hb1.type == Hitbox2D::TYPE_CIRCLE ? hb1.circleCentre().x() : (hb1.rectPos().x());
	float hb1Top = hb1.type == Hitbox2D::TYPE_CIRCLE ? hb1.circleCentre().y() : (hb1.rectPos().y());
	float hb1Bottom = hb1.type == Hitbox2D::TYPE_CIRCLE ? hb1.circleCentre().y() : (hb1.rectPos().y() + hb1.rectSize().y());

	float hb2Right = hb2.type == Hitbox2D::TYPE_CIRCLE ? hb2.circleCentre().x() : (hb2.rectPos().x() + hb2.rectSize().x());
	float hb2Left = hb2.type == Hitbox2D::TYPE_CIRCLE ? hb2.circleCentre().x() : (hb2.rectPos().x());
	float hb2Top = hb2.type == Hitbox2D::TYPE_CIRCLE ? hb2.circleCentre().y() : (hb2.rectPos().y());
	float hb2Bottom = hb2.type == Hitbox2D::TYPE_CIRCLE ? hb2.circleCentre().y() : (hb2.rectPos().y() + hb2.rectSize().y());

	int ret = COLLISION_NONE;
	if (hb1Right >= hb2Left && hb1Right <= hb2Right) {
		ret += COLLISION_RIGHT;
	}
	else if (hb1Left <= hb2Right) {
		ret += COLLISION_LEFT;
	}
	if (hb1Top <= hb2Bottom) {
		ret += COLLISION_UP;
	}
	else if (hb1Bottom >= hb2Top) {
		ret += COLLISION_DOWN;
	}
	return ret;
}

bool Physics2D::isCollisionDown(CollisionData collisionProperties)
{
	return isCollisionDown(collisionProperties.direction);
}

bool Physics2D::isCollisionUp(CollisionData collisionProperties)
{
	return isCollisionUp(collisionProperties.direction);
}

bool Physics2D::isCollisionLeft(CollisionData collisionProperties)
{
	return isCollisionLeft(collisionProperties.direction);
}

bool Physics2D::isCollisionRight(CollisionData collisionProperties)
{
	return isCollisionDown(collisionProperties.direction);
}

bool Physics2D::isCollisionDown(int collisionProperties)
{
	return collisionProperties == COLLISION_DOWN || collisionProperties == COLLISION_DOWN_LEFT || collisionProperties == COLLISION_DOWN_RIGHT;
}

bool Physics2D::isCollisionUp(int collisionProperties)
{
	return collisionProperties == COLLISION_UP || collisionProperties == COLLISION_UP_LEFT || collisionProperties == COLLISION_UP_RIGHT;
}

bool Physics2D::isCollisionLeft(int collisionProperties)
{
	return collisionProperties == COLLISION_LEFT || collisionProperties == COLLISION_UP_LEFT || collisionProperties == COLLISION_DOWN_LEFT;
}

bool Physics2D::isCollisionRight(int collisionProperties)
{
	return collisionProperties == COLLISION_RIGHT || collisionProperties == COLLISION_UP_RIGHT || collisionProperties == COLLISION_DOWN_RIGHT;
}

Physics2D::CollisionData::CollisionData(int hitDirection, Vector2 hitOverlap)
{
	direction = hitDirection;
	hitVector = hitOverlap;
}
