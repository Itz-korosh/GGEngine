#pragma once

namespace Input {

class Keyboard {
public:
    // The order matters: letters, digits and F-keys must stay in consecutive
    // runs (Keyboard.cpp relies on that), and Count must stay last.
    enum Key {
        Unknown,

        A, B, C, D, E, F, G, H, I, J, K, L, M,
        N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

        Num0, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,

        Left, Right, Up, Down,
        Space, Enter, Escape, Tab, Backspace,
        LeftShift, RightShift, LeftCtrl, RightCtrl, LeftAlt, RightAlt,

        F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,

        Count
    };

    static bool isKeyDown(Key key);       // true every frame while held
    static bool isKeyPressed(Key key);    // true only on the frame it went down
    static bool isKeyReleased(Key key);   // true only on the frame it came up

    // Called by Window once per frame, before it reads events.
    static void update();
};

} // namespace Input