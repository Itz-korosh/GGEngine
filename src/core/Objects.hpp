#pragma once

#include "Math/Vector.hpp"
#include "ui/ColorNode.hpp"

namespace Objects {

    // Pos is the top-left corner, like DrawRect.
    class Object2D {
    public:
        Vector::Vec2D Pos;
        Vector::Vec2D Size;
        UI::RGB Color;

        Object2D(UI::RGB color, Vector::Vec2D position, Vector::Vec2D size)
            : Pos(position), Size(size), Color(color) {}
    };

    // Pos is the corner with the smallest x, y, z.
    class Object3D {
    public:
        Vector::Vec3D Pos;
        Vector::Vec3D Size;
        UI::RGB Color;

        Object3D(UI::RGB color, Vector::Vec3D position, Vector::Vec3D size)
            : Pos(position), Size(size), Color(color) {}
    };

} // namespace Objects