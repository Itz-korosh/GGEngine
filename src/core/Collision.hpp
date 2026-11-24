#pragma once
#include "Objects.hpp";

namespace Collision {
    bool Collide2D(Objects::Object2D obj1, Objects::Object2D obj2);
    bool Collide3D(Objects::Object3D obj1, Objects::Object3D obj2);
}