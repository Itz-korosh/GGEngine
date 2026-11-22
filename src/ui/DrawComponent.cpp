#include "ui/DrawComponent.hpp"
#include "ui/Window.hpp"

#include <glad/glad.h>

namespace UI {

namespace {
    // Sets OpenGL's current drawing color.
    void SetColor(const RGBA& color)
    {
        GLRGBA c = color.ToGL();
        glColor4f(c.r, c.g, c.b, c.a);
    }

    // An RGB is just an RGBA that is fully visible.
    RGBA ToRGBA(const RGB& c)
    {
        return RGBA(c.r, c.g, c.b, 255);
    }
}

DrawComponent::DrawComponent(const Window& window)
    : m_window(window)
{
    // Make alpha (transparency) work.
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

// Maps OpenGL's coordinates to pixels: (0,0) top-left, (width,height) bottom-right.
void DrawComponent::SetupProjection() const
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, m_window.getWidth(), m_window.getHeight(), 0.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// ---- ClearScreen ----

void DrawComponent::ClearScreen()
{
    ClearScreen(RGBA(0, 0, 0, 255));
}

void DrawComponent::ClearScreen(const RGB& color)
{
    ClearScreen(ToRGBA(color));
}

void DrawComponent::ClearScreen(const RGBA& color)
{
    GLRGBA c = color.ToGL();
    glClearColor(c.r, c.g, c.b, c.a);
    glClear(GL_COLOR_BUFFER_BIT);
}

// ---- DrawPixel ----

void DrawComponent::DrawPixel(const Vector::Vec2D& position, const RGB& color)
{
    DrawPixel(position, ToRGBA(color));
}

void DrawComponent::DrawPixel(const Vector::Vec2D& position, const RGBA& color)
{
    SetupProjection();
    SetColor(color);
    glPointSize(1.0f);
    glBegin(GL_POINTS);
        glVertex2f(position.x + 0.5f, position.y + 0.5f);   // +0.5 = center of the pixel
    glEnd();
}

// ---- DrawRect ----

void DrawComponent::DrawRect(const Vector::Vec2D& position, float width, float height, const RGB& color)
{
    DrawRect(position, width, height, ToRGBA(color));
}

void DrawComponent::DrawRect(const Vector::Vec2D& position, float width, float height, const RGBA& color)
{
    SetupProjection();
    SetColor(color);
    glBegin(GL_QUADS);
        glVertex2f(position.x,         position.y);
        glVertex2f(position.x + width, position.y);
        glVertex2f(position.x + width, position.y + height);
        glVertex2f(position.x,         position.y + height);
    glEnd();
}

// ---- DrawTriangle ----

void DrawComponent::DrawTriangle(const Vector::Vec3D& a, const Vector::Vec3D& b, const Vector::Vec3D& c, const RGB& color)
{
    DrawTriangle(a, b, c, ToRGBA(color));
}

void DrawComponent::DrawTriangle(const Vector::Vec3D& a, const Vector::Vec3D& b, const Vector::Vec3D& c, const RGBA& color)
{
    SetupProjection();
    SetColor(color);
    glBegin(GL_TRIANGLES);
        glVertex2f(a.x, a.y);   // z is ignored for now (see notes)
        glVertex2f(b.x, b.y);
        glVertex2f(c.x, c.y);
    glEnd();
}

} // namespace UI