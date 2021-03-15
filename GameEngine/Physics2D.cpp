#include <cmath>
#include "Log.h"
#include "Physics2D.h"

using namespace ingenium2D;

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
	return colliding(hb1, hb2, Vector2(), Vector2());
}

Physics2D::CollisionData Physics2D::colliding(Hitbox2D hb1, Hitbox2D hb2, Vector2 hb1Speed, Vector2 hb2Speed)
{
	if (hb1.type == Hitbox2D::TYPE_UNDEFINED || hb2.type == Hitbox2D::TYPE_UNDEFINED)
		return CollisionData(COLLISION_NONE, DEFAULT_COLLISION_DIST);

	hb1.position.add(hb1Speed);
	hb2.position.add(hb2Speed);

	// CIRCLE - CIRCLE COLLISION
	if (hb1.type == Hitbox2D::TYPE_CIRCLE && hb2.type == Hitbox2D::TYPE_CIRCLE) {
		Vector2 posDifference = Vector2::subtract(hb1.circleCentre(), hb2.circleCentre());
		float combinedRadii = hb1.circleRadius() + hb2.circleRadius();
		float distance = Vector::qSqrt(posDifference.x * posDifference.x + posDifference.y * posDifference.y);
		if (distance <= combinedRadii) {

			float r = combinedRadii - distance;
			float theta = std::atan(posDifference.y / posDifference.x);
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
	// RECTANGLE - RECTANGLE COLLISION
	if (hb1.type == Hitbox2D::TYPE_RECTANGLE && hb2.type == Hitbox2D::TYPE_RECTANGLE) {
		if (hb1.position.x <= hb2.position.x + hb2.rectSize().x &&
			hb1.position.x + hb1.rectSize().x >= hb2.position.x &&
			hb1.position.y <= hb2.position.y + hb2.rectSize().y &&
			hb1.position.y + hb1.rectSize().y >= hb2.position.y) {
			Vector2 hitVector = Vector2();
			int hitDirec = hitDirection(hb1, hb2);

			Vector2 hb1Center = Vector2(hb1.position.x + hb1.rectSize().x * 0.5, hb1.position.y + hb1.rectSize().y * 0.5);
			Vector2 hb2Center = Vector2(hb2.position.x + hb2.rectSize().x * 0.5, hb2.position.y + hb2.rectSize().y * 0.5);

			float errMargin = Vector2::add(hb1Speed, hb2Speed).y;
			float correction = 3;
			errMargin = (errMargin < 0 ? (-errMargin + correction) : (errMargin + correction));

			if (hb1.position.y + hb1.rectSize().y <= hb2.position.y + errMargin || hb1.position.y + errMargin >= hb2.position.y + hb2.rectSize().y) {
				if (hb1Center.y < hb2Center.y) { // Relatively above
					hitDirec = COLLISION_UP;
				}
				else { // Relatively below
					hitDirec = COLLISION_DOWN;
				}
			}
			else {
				if (hb1Center.x > hb2Center.x) { // Relatively left
					hitDirec = COLLISION_LEFT;
				}
				else { // Relatively right
					hitDirec = COLLISION_RIGHT;
				}
			}

			return CollisionData(hitDirec, hitVector);
		}

		return CollisionData(COLLISION_NONE, DEFAULT_COLLISION_DIST);
	}
	// RECTANGLE - CIRCLE COLLISION
	if ((hb1.type == Hitbox2D::TYPE_CIRCLE && hb2.type == Hitbox2D::TYPE_RECTANGLE) || (hb1.type == Hitbox2D::TYPE_RECTANGLE && hb2.type == Hitbox2D::TYPE_CIRCLE)) {
		bool swap = false;
		if (hb2.type == Hitbox2D::TYPE_CIRCLE) {
			Hitbox2D circle = hb2;
			Hitbox2D rect = hb1;
			hb1 = circle;
			hb2 = rect;
			swap = true;
		}
		float testX = hb1.circleCentre().x;
		float testY = hb1.circleCentre().y;

		if (hb1.circleCentre().x < hb2.rectPos().x) {
			testX = hb2.rectPos().x;
		}
		else if (hb1.circleCentre().x > hb2.rectPos().x + hb2.rectSize().x) {
			testX = hb2.rectPos().x + hb2.rectSize().x;
		}
		if (hb1.circleCentre().y < hb2.rectPos().y) {
			testY = hb2.rectPos().y;
		}
		else if (hb1.circleCentre().y > hb2.rectPos().y + hb2.rectSize().y) {
			testY = hb2.rectPos().y + hb2.rectSize().y;
		}

		Vector2 distanceVector = Vector2(hb1.circleCentre().x - testX, hb1.circleCentre().y - testY);
		float distance = Vector2::hypotenuse(distanceVector);


		if (distance <= hb1.circleRadius()) { // If there is a collision

			float L = hb2.rectPos().x; // Rectangle left side
			float T = hb2.rectPos().y; // Rectangle top side
			float R = hb2.rectPos().x + hb2.rectSize().x; // Rectangle right side
			float B = hb2.rectPos().y + hb2.rectSize().y; // Rectangle bottom side

			Vector2 hitVec = Vector2(); // The vector2 quantity repersenting the amount of overlap between the circle and rectangle to allow for easy collision resolution

			// Hit direction - not used in calculations
			int hitDir = hitDirection(hb1, hb2);
			if (swap)
				hitDir = hitDirection(hb2, hb1);

			// Checking for side or corner collisions
			if (hb1.circleCentre().x > L && hb1.circleCentre().x < R) {
				// Side collision : top or bottom
				if (hb1.circleCentre().y < T) {
					// Top side
					hitVec.y = (hb1.circleCentre().y + hb1.circleRadius()) - hb2.rectPos().y;
				}
				else {
					// Bottom side
					hitVec.y = (hb1.circleCentre().y - hb1.circleRadius()) - (hb2.rectPos().y + hb2.rectSize().y);
				}
				return CollisionData(hitDir, hitVec);
			}
			else if (hb1.circleCentre().y > T && hb1.circleCentre().y < B) {
				// Side collision : left or right
				if (hb1.circleCentre().x < L) {
					// Left side
					hitVec.x = (hb1.circleCentre().x + hb1.circleRadius()) - hb2.rectPos().x;
				}
				else {
					// Right side
					hitVec.x = ((hb1.circleCentre().x - hb1.circleRadius()) - (hb2.rectPos().x + hb2.rectSize().x));
				}
				return CollisionData(hitDir, hitVec);
			}
			// TODO: find the proper values to populate the hit vector with on corner collisions
			else if (hb1.circleCentre().x < L) {
				// Corner collison : left corners

				float r = hb1.circleRadius(); // Radius of circle
				float cX = hb1.position.x; // X position of circle center
				float cY = hb1.position.y; // Y position of circle center

				if (hb1.circleCentre().y < T) {
					// Top left corner collision
					float a = hb2.rectUL().y - hb1.circleCentre().y; // Y value of triangle formed between circle center and corner
					float b = hb2.rectUL().x - hb1.circleCentre().x; // X value of triangle formed between circle center and corner
					float c = Vector::qSqrt(a * a + b * b); // Hypoteunse of the triangle formed between circle center and corner
				}
				else {
					float a = hb2.rectBL().y - hb1.circleCentre().y; // Y value of triangle formed between circle center and corner
					float b = hb2.rectBL().x - hb1.circleCentre().x; // X value of triangle formed between circle center and corner
					float c = Vector::qSqrt(a * a + b * b); // Hypoteunse of the triangle formed between circle center and corner
					// Bottom left corner collision
				}
				return CollisionData(hitDir, hitVec);
			}
			else if (hb1.circleCentre().x > R) {
				// Corner collision : right corners

				float r = hb1.circleRadius(); // Radius of circle
				float cX = hb1.position.x; // X position of circle center
				float cY = hb1.position.y; // Y position of circle center

				if (hb1.circleCentre().y < T) {
					// Top right corner collision
					float a = hb2.rectUR().y - hb1.circleCentre().y; // Y value of triangle formed between circle center and corner
					float b = hb2.rectUR().x - hb1.circleCentre().x; // X value of triangle formed between circle center and corner
					float c = Vector::qSqrt(a * a + b * b); // Hypoteunse of the triangle formed between circle center and corner
				}
				else {
					// Bottom right corner collision

					float a = hb2.rectBR().y - hb1.circleCentre().y; // Y value of triangle formed between circle center and corner
					float b = hb2.rectBR().x - hb1.circleCentre().x; // X value of triangle formed between circle center and corner
					float c = Vector::qSqrt(a * a + b * b); // Hypoteunse of the triangle formed between circle center and corner
				}
				return CollisionData(hitDir, hitVec);
			}
		}
	}
	return CollisionData(COLLISION_NONE, DEFAULT_COLLISION_DIST);
}

int Physics2D::hitDirection(Hitbox2D hb1, Hitbox2D hb2)
{
	float hb1Right = hb1.type == Hitbox2D::TYPE_CIRCLE ? hb1.circleCentre().x : (hb1.rectPos().x + hb1.rectSize().x);
	float hb1Left = hb1.type == Hitbox2D::TYPE_CIRCLE ? hb1.circleCentre().x : (hb1.rectPos().x);
	float hb1Top = hb1.type == Hitbox2D::TYPE_CIRCLE ? hb1.circleCentre().y : (hb1.rectPos().y);
	float hb1Bottom = hb1.type == Hitbox2D::TYPE_CIRCLE ? hb1.circleCentre().y : (hb1.rectPos().y + hb1.rectSize().y);

	float hb2Right = hb2.type == Hitbox2D::TYPE_CIRCLE ? hb2.circleCentre().x : (hb2.rectPos().x + hb2.rectSize().x);
	float hb2Left = hb2.type == Hitbox2D::TYPE_CIRCLE ? hb2.circleCentre().x : (hb2.rectPos().x);
	float hb2Top = hb2.type == Hitbox2D::TYPE_CIRCLE ? hb2.circleCentre().y : (hb2.rectPos().y);
	float hb2Bottom = hb2.type == Hitbox2D::TYPE_CIRCLE ? hb2.circleCentre().y : (hb2.rectPos().y + hb2.rectSize().y);

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
