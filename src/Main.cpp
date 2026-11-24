#include "ui/Window.hpp"
#include "ui/DrawComponent.hpp"
#include "ui/ColorNode.hpp"
#include "Math/Vector.hpp"

int main() {
    UI::Window win;
    UI::DrawComponent draw(win);
    win.setTitle("My Game");
    win.setSize(800, 600);
    win.center();
    win.setResizable(false);

    Vector::Vec2D pos1;
    pos1.x = 100;
    pos1.y = 200;
    win.run([&]() {
        draw.ClearScreen(UI::RGB(0,255,0));
        draw.DrawRect(pos1,50,100,UI::RGB(255,0,0));
    });
}
