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

    win.run([&]() {
        draw.DrawRect(100,200,50,50,UI::RGB(255,0,0));
        draw.ClearScreen(UI::RGB(0,255,0));
    });
}
