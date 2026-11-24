#include <string>

#include "ui/Window.hpp"
#include "ui/DrawComponent.hpp"
#include "ui/ColorNode.hpp"
#include "Math/Vector.hpp"
#include "Math/Random.hpp"
#include "input/Keyboard.hpp"
#include "input/Mouse.hpp"
#include "time/Time.hpp"
#include "time/Timer.hpp"

int main() {
    UI::Window win("Mouse Test", 800, 600);
    UI::DrawComponent draw(win);
    Input::Mouse mouse;
    Input::Keyboard kb;

    win.center();
    win.setResizable(false);
    win.setBackground(UI::RGB(30, 30, 40));

    // The draggable box
    Vector::Vec2D boxPos{340.0f, 260.0f};
    Vector::Vec2D grabOffset{0.0f, 0.0f};
    UI::RGB boxColor(255, 128, 0);
    float scale = 1.0f;
    bool dragging = false;
    bool cursorVisible = true;

    Timing::Timer flash(0.2f);          // white flash when you let go of the box
    Timing::Timer titleTimer(0.1f);     // updates the title text 10x per second
    titleTimer.start();

    win.run([&](float) {
        Vector::Vec2D m = mouse.getPosition();
        float w = 120.0f * scale;
        float h = 80.0f * scale;

        bool overBox = m.x >= boxPos.x && m.x <= boxPos.x + w &&
                       m.y >= boxPos.y && m.y <= boxPos.y + h;

        // Left button: drag the box
        if (mouse.isButtonPressed(Input::Mouse::Left) && overBox) {
            dragging = true;
            grabOffset.x = m.x - boxPos.x;
            grabOffset.y = m.y - boxPos.y;
        }
        if (dragging) {
            boxPos.x = m.x - grabOffset.x;
            boxPos.y = m.y - grabOffset.y;
        }
        if (mouse.isButtonReleased(Input::Mouse::Left)) {
            if (dragging) flash.start();
            dragging = false;
        }

        // Right button: random color
        if (mouse.isButtonPressed(Input::Mouse::Right)) {
            boxColor = UI::RGB(Math::Random::nextInt(0, 255),
                               Math::Random::nextInt(0, 255),
                               Math::Random::nextInt(0, 255));
        }

        // Middle button: reset everything
        if (mouse.isButtonPressed(Input::Mouse::Middle)) {
            boxPos = Vector::Vec2D{340.0f, 260.0f};
            boxColor = UI::RGB(255, 128, 0);
            scale = 1.0f;
        }

        // Wheel: resize
        scale += mouse.getScrollY() * 0.1f;
        if (scale < 0.3f) scale = 0.3f;
        if (scale > 4.0f) scale = 4.0f;

        // H: hide or show the system cursor, Esc: quit
        if (kb.isKeyPressed(Input::Keyboard::H)) {
            cursorVisible = !cursorVisible;
            mouse.setCursorVisible(cursorVisible);
        }
        if (kb.isKeyPressed(Input::Keyboard::Escape)) win.close();

        // ---- Drawing ----
        bool flashing = flash.isRunning() && !flash.isFinished();

        if (overBox)   // white outline while hovering
            draw.DrawRect(Vector::Vec2D{boxPos.x - 3.0f, boxPos.y - 3.0f}, w + 6.0f, h + 6.0f, UI::RGB(255, 255, 255));

        draw.DrawRect(boxPos, w, h, flashing ? UI::RGB(255, 255, 255) : boxColor);

        // Button indicators (bottom left): Left, Middle, Right
        draw.DrawRect(Vector::Vec2D{20.0f, 550.0f}, 30, 30,
                      mouse.isButtonDown(Input::Mouse::Left)   ? UI::RGB(255, 80, 80)  : UI::RGB(70, 70, 80));
        draw.DrawRect(Vector::Vec2D{60.0f, 550.0f}, 30, 30,
                      mouse.isButtonDown(Input::Mouse::Middle) ? UI::RGB(80, 255, 80)  : UI::RGB(70, 70, 80));
        draw.DrawRect(Vector::Vec2D{100.0f, 550.0f}, 30, 30,
                      mouse.isButtonDown(Input::Mouse::Right)  ? UI::RGB(80, 160, 255) : UI::RGB(70, 70, 80));

        // Our own cursor dot (visible even when the system cursor is hidden)
        draw.DrawRect(Vector::Vec2D{m.x - 4.0f, m.y - 4.0f}, 8, 8, UI::RGB(255, 255, 255));

        // Live info in the title bar
        if (titleTimer.isFinished()) {
            win.setTitle("x:" + std::to_string((int)m.x) +
                         " y:" + std::to_string((int)m.y) +
                         " | dx:" + std::to_string((int)mouse.getDeltaX()) +
                         " dy:" + std::to_string((int)mouse.getDeltaY()) +
                         " | scale:" + std::to_string((int)(scale * 100)) + "%" +
                         " | FPS:" + std::to_string((int)Timing::Time::getFps()));
            titleTimer.start();
        }
    });
}