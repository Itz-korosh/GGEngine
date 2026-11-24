#include "core/Collision.hpp"

namespace Collision {

// Two boxes overlap when they overlap on every axis.
bool Collide2D(const Objects::Object2D& a, const Objects::Object2D& b)
{
    bool overlapX = a.Pos.x < b.Pos.x + b.Size.x && a.Pos.x + a.Size.x > b.Pos.x;
    bool overlapY = a.Pos.y < b.Pos.y + b.Size.y && a.Pos.y + a.Size.y > b.Pos.y;
    return overlapX && overlapY;
}

bool Collide3D(const Objects::Object3D& a, const Objects::Object3D& b)
{
    bool overlapX = a.Pos.x < b.Pos.x + b.Size.x && a.Pos.x + a.Size.x > b.Pos.x;
    bool overlapY = a.Pos.y < b.Pos.y + b.Size.y && a.Pos.y + a.Size.y > b.Pos.y;
    bool overlapZ = a.Pos.z < b.Pos.z + b.Size.z && a.Pos.z + a.Size.z > b.Pos.z;
    return overlapX && overlapY && overlapZ;
}

} // namespace Collision