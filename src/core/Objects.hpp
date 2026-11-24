#pragma once

#include "Math/Vector.hpp"
#include "ui/ColorNode.hpp"

namespace Objects {
    class Object2D {
    public:
        Vector::Vec2D Pos;
        UI::RGB Color;

        Object2D(UI::RGB color, Vecotr::Vec2D position) {
            this.Color = color;
            this.Pos = position;
            
        }
    }

    class Object3D {
    public:
        Vector::Vec3D Pos;
        UI::RGB Color;

        Object3D(UI::RGB color, Vecotr::Vec3D position) {
            this.Color = color;
            this.Pos = position;
            
        }
    }
}
