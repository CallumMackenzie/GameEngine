#pragma once

#include "Vector.h"
#include "Vector2.h"
#include "Rotation.h"

// Callum Mackenzie

namespace ingenium2D 
{
    class GameObject
    {
    public:
        GameObject(const char* name_, Vector2 pos, Rotation rot);
        GameObject(const char* name_);
        GameObject(const char* name_, Vector2 pos);
        GameObject(const char* name_, Rotation rot);
        ~GameObject();

    public:
        Vector2 position = Vector2();
        Vector2 scale = Vector2(1, 1);
        Rotation rotation = Rotation();
        const char* name;
        // const char* group;
    };
};