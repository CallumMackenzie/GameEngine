#include "GameObject.h"


// Callum Mackenzie

GameObject::GameObject(const char* name_, Vector2 pos, Rotation rot)
{
	name = name_;
	position = Vector2::asPtr(pos);
	rotation = Rotation::asPtr(rot);
}

GameObject::GameObject(const char* name_) : GameObject(name_, Vector2(), Rotation())
{ }

GameObject::GameObject(const char* name_, Vector2 pos) : GameObject(name_, pos, Rotation())
{ }

GameObject::GameObject(const char* name_, Rotation rot) : GameObject(name_, Vector2(), rot)
{ }

GameObject::~GameObject()
{
	delete rotation;
	delete position;
}
