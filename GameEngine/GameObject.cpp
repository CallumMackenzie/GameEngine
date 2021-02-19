#include "GameObject.h"


// Callum Mackenzie

GameObject::GameObject(const char* name_, Vector2* pos, Rotation* rot)
{
	name = name_;
	position = pos;
	rotation = rot;
}

GameObject::GameObject(const char* name_) : GameObject(name_, new Vector2(), new Rotation())
{ }

GameObject::GameObject(const char* name_, Vector2* pos) : GameObject(name_, pos, new Rotation())
{ }

GameObject::GameObject(const char* name_, Rotation* rot) : GameObject(name_, new Vector2(), rot)
{ }
