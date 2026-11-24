#include "input/Mouse.hpp"

#include <SDL3/SDL.h>

namespace Input {

namespace {

bool  s_prevButtons[Mouse::Count] = {};
float s_prevX = 0.0f;
float s_prevY = 0.0f;
float s_scrollX = 0.0f;
float s_scrollY = 0.0f;

SDL_MouseButtonFlags toMask(Mouse::Button button)
{
    switch (button) {
        case Mouse::Left:    return SDL_BUTTON_MASK(SDL_BUTTON_LEFT);
        case Mouse::Middle:  return SDL_BUTTON_MASK(SDL_BUTTON_MIDDLE);
        case Mouse::Right:   return SDL_BUTTON_MASK(SDL_BUTTON_RIGHT);
        case Mouse::Back:    return SDL_BUTTON_MASK(SDL_BUTTON_X1);
        case Mouse::Forward: return SDL_BUTTON_MASK(SDL_BUTTON_X2);
        default:             return 0;
    }
}

} // namespace

bool Mouse::isButtonDown(Button button)
{
    return (SDL_GetMouseState(nullptr, nullptr) & toMask(button)) != 0;
}

bool Mouse::isButtonPressed(Button button)
{
    return isButtonDown(button) && !s_prevButtons[button];
}

bool Mouse::isButtonReleased(Button button)
{
    return !isButtonDown(button) && s_prevButtons[button];
}

float Mouse::getX()
{
    float x = 0.0f, y = 0.0f;
    SDL_GetMouseState(&x, &y);
    return x;
}

float Mouse::getY()
{
    float x = 0.0f, y = 0.0f;
    SDL_GetMouseState(&x, &y);
    return y;
}

Vector::Vec2D Mouse::getPosition()
{
    Vector::Vec2D position;
    position.x = getX();
    position.y = getY();
    return position;
}

float Mouse::getDeltaX() { return getX() - s_prevX; }
float Mouse::getDeltaY() { return getY() - s_prevY; }

float Mouse::getScrollX() { return s_scrollX; }
float Mouse::getScrollY() { return s_scrollY; }

void Mouse::setCursorVisible(bool visible)
{
    if (visible) SDL_ShowCursor();
    else         SDL_HideCursor();
}

void Mouse::update()
{
    for (int i = 0; i < Count; ++i)
        s_prevButtons[i] = isButtonDown(static_cast<Button>(i));

    s_prevX = getX();
    s_prevY = getY();

    s_scrollX = 0.0f;   // the wheel total starts fresh every frame
    s_scrollY = 0.0f;
}

void Mouse::onScroll(float x, float y)
{
    s_scrollX += x;
    s_scrollY += y;
}

} // namespace Input