#pragma once

#include "Vector.h"
#include "Vector2.h"
#include "Rotation.h"

// Callum Mackenzie

// TODO: Game object class

class GameObject 
{
public:
    GameObject(const char* name_);
    GameObject(const char* name_, Vector2* pos);
    GameObject(const char* name_, Vector2* pos, Rotation* rot);
    GameObject(const char* name_, Rotation* rot);

public:
    Vector2* position = nullptr;
    Rotation* rotation = nullptr;
    const char* name;
};