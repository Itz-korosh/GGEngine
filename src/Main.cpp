#include "ui/Window.hpp"
#include "ui/DrawComponent.hpp"
#include "ui/ColorNode.hpp"
#include "Math/Vector.hpp"
#include "input/Keyboard.hpp"

int main() {
    UI::Window win;
    UI::DrawComponent draw(win);
    Input::Keyboard kbi;

    win.setTitle("My Game");
    win.setSize(800, 600);
    win.center();
    win.setResizable(false);

    Vector::Vec2D pos1;
    pos1.x = 100;
    pos1.y = 200;

    UI::RGB objcolor(255, 0, 0);

    win.run([&]() {
        draw.ClearScreen(UI::RGB(0, 255, 0));

        if (kbi.isKeyDown(Input::Keyboard::Left)) {
            objcolor = UI::RGB(0, 0, 255);    
        } else {
            objcolor = UI::RGB(255, 0, 0);    
        }

        draw.DrawRect(pos1, 50, 100, objcolor);
    });
}