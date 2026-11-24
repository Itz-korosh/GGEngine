#include "Objects.hpp"

bool Collide2D(Objects::Object2D obj1, Objects::Object2D obj2) {
    if (obj1.Pos.x = obj2.Pos.y) {
        if (obj1.Pos.y = obj2.Pos.y) {return true;}
    }
    else {return false}
}

bool Collide3D(Objects::Object3D obj1, Objects::Object3D obj2) {
    if (obj1.Pos.x = obj2.Pos.y) {
        if (obj1.Pos.y = obj2.Pos.y) {
            if (obj1.Pos.z = obj2.Pos.z) {return true}
        }
    }
    else {return false}
}