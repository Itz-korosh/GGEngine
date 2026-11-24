#include "keyboard/Keyboard.hpp"

#include <SDL3/SDL.h>

namespace Input {

namespace {

// What was held down at the start of the last frame, used for pressed/released.
bool s_previous[static_cast<int>(Key::Count)] = {};

int toIndex(Key key) { return static_cast<int>(key); }

// Converts our Key to SDL's scancode (the physical key position).
SDL_Scancode toScancode(Key key)
{
    if (key >= Key::A && key <= Key::Z)
        return static_cast<SDL_Scancode>(SDL_SCANCODE_A + (toIndex(key) - toIndex(Key::A)));

    if (key >= Key::Num1 && key <= Key::Num9)
        return static_cast<SDL_Scancode>(SDL_SCANCODE_1 + (toIndex(key) - toIndex(Key::Num1)));

    if (key >= Key::F1 && key <= Key::F12)
        return static_cast<SDL_Scancode>(SDL_SCANCODE_F1 + (toIndex(key) - toIndex(Key::F1)));

    switch (key) {
        case Key::Num0:       return SDL_SCANCODE_0;
        case Key::Left:       return SDL_SCANCODE_LEFT;
        case Key::Right:      return SDL_SCANCODE_RIGHT;
        case Key::Up:         return SDL_SCANCODE_UP;
        case Key::Down:       return SDL_SCANCODE_DOWN;
        case Key::Space:      return SDL_SCANCODE_SPACE;
        case Key::Enter:      return SDL_SCANCODE_RETURN;
        case Key::Escape:     return SDL_SCANCODE_ESCAPE;
        case Key::Tab:        return SDL_SCANCODE_TAB;
        case Key::Backspace:  return SDL_SCANCODE_BACKSPACE;
        case Key::LeftShift:  return SDL_SCANCODE_LSHIFT;
        case Key::RightShift: return SDL_SCANCODE_RSHIFT;
        case Key::LeftCtrl:   return SDL_SCANCODE_LCTRL;
        case Key::RightCtrl:  return SDL_SCANCODE_RCTRL;
        case Key::LeftAlt:    return SDL_SCANCODE_LALT;
        case Key::RightAlt:   return SDL_SCANCODE_RALT;
        default:              return SDL_SCANCODE_UNKNOWN;
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
    return isKeyDown(key) && !s_previous[toIndex(key)];
}

bool Keyboard::isKeyReleased(Key key)
{
    return !isKeyDown(key) && s_previous[toIndex(key)];
}

void Keyboard::update()
{
    for (int i = 0; i < toIndex(Key::Count); ++i)
        s_previous[i] = isKeyDown(static_cast<Key>(i));
}

} // namespace Input