#pragma once

#include "Math/Vector.hpp"
#include "ui/ColorNode.hpp"

namespace UI {

class Window;   // forward declaration

class DrawComponent {
public:
    // The window must already exist (it provides the OpenGL context and size).
    explicit DrawComponent(const Window& window);

    // Clears the screen. Call it at the start of every frame.
    void ClearScreen();                     // black
    void ClearScreen(const RGB& color);
    void ClearScreen(const RGBA& color);

    void DrawPixel(const Vector::Vec2D& position, const RGB& color);
    void DrawPixel(const Vector::Vec2D& position, const RGBA& color);

    // position = top-left corner
    void DrawRect(const Vector::Vec2D& position, float width, float height, const RGB& color);
    void DrawRect(const Vector::Vec2D& position, float width, float height, const RGBA& color);

    // a, b, c = the triangle's three corners
    void DrawTriangle(const Vector::Vec3D& a, const Vector::Vec3D& b, const Vector::Vec3D& c, const RGB& color);
    void DrawTriangle(const Vector::Vec3D& a, const Vector::Vec3D& b, const Vector::Vec3D& c, const RGBA& color);

private:
    void SetupProjection() const;

    const Window& m_window;
};

} // namespace UI