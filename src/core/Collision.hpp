#pragma once

#include "core/Objects.hpp"

namespace Collision {
    bool Collide2D(const Objects::Object2D& obj1, const Objects::Object2D& obj2);
    bool Collide3D(const Objects::Object3D& obj1, const Objects::Object3D& obj2);
}