#pragma once

#include "Math/Vector.hpp"

namespace Input {

class Mouse {
public:
    enum Button { Left, Middle, Right, Back, Forward, Count };

    static bool isButtonDown(Button button);       // true every frame while held
    static bool isButtonPressed(Button button);    // true only on the frame it went down
    static bool isButtonReleased(Button button);   // true only on the frame it came up

    // Position inside the window, in pixels. (0, 0) is the top-left corner.
    static float getX();
    static float getY();
    static Vector::Vec2D getPosition();

    // How far the mouse moved during this frame.
    static float getDeltaX();
    static float getDeltaY();

    // Mouse wheel movement during this frame. Positive Y = scrolled up (away from you).
    static float getScrollX();
    static float getScrollY();

    static void setCursorVisible(bool visible);

    // Used by Window. Game code never calls these.
    static void update();
    static void onScroll(float x, float y);
};

} // namespace Input