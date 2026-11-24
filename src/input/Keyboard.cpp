#include "input/Keyboard.hpp"

#include <SDL3/SDL.h>

namespace Input {

using Key = Keyboard::Key;

namespace {

// What was held down at the start of the last frame, used for pressed/released.
bool s_previous[Keyboard::Count] = {};

// Converts our Key to SDL's scancode (the physical key position).
SDL_Scancode toScancode(Key key)
{
    if (key >= Keyboard::A && key <= Keyboard::Z)
        return static_cast<SDL_Scancode>(SDL_SCANCODE_A + (key - Keyboard::A));

    if (key >= Keyboard::Num1 && key <= Keyboard::Num9)
        return static_cast<SDL_Scancode>(SDL_SCANCODE_1 + (key - Keyboard::Num1));

    if (key >= Keyboard::F1 && key <= Keyboard::F12)
        return static_cast<SDL_Scancode>(SDL_SCANCODE_F1 + (key - Keyboard::F1));

    switch (key) {
        case Keyboard::Num0:       return SDL_SCANCODE_0;
        case Keyboard::Left:       return SDL_SCANCODE_LEFT;
        case Keyboard::Right:      return SDL_SCANCODE_RIGHT;
        case Keyboard::Up:         return SDL_SCANCODE_UP;
        case Keyboard::Down:       return SDL_SCANCODE_DOWN;
        case Keyboard::Space:      return SDL_SCANCODE_SPACE;
        case Keyboard::Enter:      return SDL_SCANCODE_RETURN;
        case Keyboard::Escape:     return SDL_SCANCODE_ESCAPE;
        case Keyboard::Tab:        return SDL_SCANCODE_TAB;
        case Keyboard::Backspace:  return SDL_SCANCODE_BACKSPACE;
        case Keyboard::LeftShift:  return SDL_SCANCODE_LSHIFT;
        case Keyboard::RightShift: return SDL_SCANCODE_RSHIFT;
        case Keyboard::LeftCtrl:   return SDL_SCANCODE_LCTRL;
        case Keyboard::RightCtrl:  return SDL_SCANCODE_RCTRL;
        case Keyboard::LeftAlt:    return SDL_SCANCODE_LALT;
        case Keyboard::RightAlt:   return SDL_SCANCODE_RALT;
        default:                   return SDL_SCANCODE_UNKNOWN;
    }
}

} // namespace

bool Keyboard::isKeyDown(Key key)
{
    SDL_Scancode scancode = toScancode(key);
    if (scancode == SDL_SCANCODE_UNKNOWN) return false;

    const bool* state = SDL_GetKeyboardState(nullptr);
    return state && state[scancode];
}

bool Keyboard::isKeyPressed(Key key)
{
    return isKeyDown(key) && !s_previous[key];
}

bool Keyboard::isKeyReleased(Key key)
{
    return !isKeyDown(key) && s_previous[key];
}

void Keyboard::update()
{
    for (int i = 0; i < Keyboard::Count; ++i)
        s_previous[i] = isKeyDown(static_cast<Key>(i));
}

} // namespace Input