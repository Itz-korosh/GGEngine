#include <string>
#include "ui/Window.hpp"
#include "ui/DrawComponent.hpp"
#include "ui/ColorNode.hpp"
#include "Math/Vector.hpp"
#include "Math/Random.hpp"
#include "input/Keyboard.hpp"
#include "time/Time.hpp"
#include "time/Timer.hpp"

int main() {
    UI::Window win;
    UI::DrawComponent draw(win);
    Input::Keyboard kb;

    win.setTitle("My Game");
    win.setSize(800, 600);
    win.center();
    win.setBackground(UI::RGB(0, 255, 0));

    Vector::Vec2D pos{100.0f, 200.0f};
    UI::RGB color(255, 0, 0);
    const float speed = 300.0f;

    Timing::Timer fpsTimer(1.0f);   // fires once per second
    fpsTimer.start();

    win.run([&](float dt) {
        if (kb.isKeyDown(Input::Keyboard::Left))  pos.x -= speed * dt;
        if (kb.isKeyDown(Input::Keyboard::Right)) pos.x += speed * dt;
        if (kb.isKeyDown(Input::Keyboard::Up))    pos.y -= speed * dt;
        if (kb.isKeyDown(Input::Keyboard::Down))  pos.y += speed * dt;

        // Space = a random new color
        if (kb.isKeyPressed(Input::Keyboard::Space)) {
            color = UI::RGB(Math::Random::nextInt(0, 255),
                            Math::Random::nextInt(0, 255),
                            Math::Random::nextInt(0, 255));
        }

        // Show the FPS in the title once per second
        if (fpsTimer.isFinished()) {
            win.setTitle("My Game | FPS: " + std::to_string((int)Timing::Time::getFps()));
            fpsTimer.start();
        }

        if (kb.isKeyPressed(Input::Keyboard::Escape)) win.close();

        draw.DrawRect(pos, 50, 100, color);
    });
}