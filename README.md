# GGEngine

A simple, open-source game engine for C++, made for people who are new to C++ and want to make games.

> **Status: pre-alpha.** GGEngine works, but it is young. The API will change, features are missing, and it has only been tested on Windows. See [Known limitations](#known-limitations).

## What a game looks like

```cpp
#include <GGEngine/GGEngine.hpp>

int main() {
    UI::Window win("My First Game", 800, 600);
    UI::DrawComponent draw(win);
    Input::Keyboard kb;

    win.setBackground(UI::RGB(30, 30, 40));

    Vector::Vec2D pos(100.0f, 100.0f);
    const float speed = 200.0f;   // pixels per second

    win.run([&](float dt) {
        if (kb.isKeyDown(Input::Keyboard::Left))  pos.x -= speed * dt;
        if (kb.isKeyDown(Input::Keyboard::Right)) pos.x += speed * dt;
        if (kb.isKeyDown(Input::Keyboard::Up))    pos.y -= speed * dt;
        if (kb.isKeyDown(Input::Keyboard::Down))  pos.y += speed * dt;

        draw.DrawRect(pos, 50, 50, UI::RGB(255, 128, 0));
    });
}
```

That is a complete game: a window with an orange square you can move with the arrow keys.

## Features

- **Window:** title, size, position, fullscreen, VSync, with a one-call game loop (`win.run(...)`)
- **2D drawing:** pixels, rectangles and triangles, in RGB or RGBA (with transparency)
- **Keyboard and mouse:** held, just-pressed and just-released checks, mouse position, movement and scroll wheel
- **Time:** delta time, FPS, elapsed time, time scale, and `Timer` objects
- **Math:** `Vec2D` and `Vec3D` with operators (`a + b`, `v * 2.0f`, `length()`, `normalized()`, `dot()`, ...) and a `Random` class
- **Collision:** box-vs-box overlap checks in 2D and 3D
- **Few dependencies:** only SDL3 (windowing and input) and GLAD (OpenGL loader), and OpenGL 2.1 from your graphics driver

## Known limitations

GGEngine is pre-alpha. Before you build something big on it, know that:

- **Windows only for now**, built with MinGW (`g++`). It was developed on Windows 7. Linux and macOS are not supported yet.
- **No text, images/sprites or audio yet.**
- **2D drawing only.** `Vec3D` and `Object3D` exist, but there is no 3D rendering yet.
- **Drawing uses simple OpenGL 2.1 immediate mode.** It is fine for hundreds of shapes, not for tens of thousands.
- **The API will change.** For example, the `DrawComponent` functions (`DrawRect`, ...) will be renamed to match the rest of the engine (`drawRect`, ...), and a simpler `App` wrapper is planned.
- **One window per program.** Keyboard and mouse state is global.

## Getting started

1. Get the **GGEngine SDK** (a folder with headers and a static library, like SDL's). Download it from the project's Releases page, or [build it yourself](docs/getting-started.md#building-the-sdk-from-source).
2. Follow **[Getting started](docs/getting-started.md)** to set up your first game. It takes about five minutes.
3. Look things up in the **[API reference](docs/api-reference.md)**.

### Requirements

- Windows
- MinGW-w64 `g++` (C++17) and GNU make (`mingw32-make`)
- A graphics driver with **OpenGL 2.1** support (a normal GPU driver; a virtual machine without 3D acceleration will not work)

## Documentation

| Document | What it covers |
|---|---|
| [Getting started](docs/getting-started.md) | Setup, your first game, core concepts, troubleshooting |
| [API reference](docs/api-reference.md) | Every class and function |

## Project layout

```
GGEngine/
├── src/            engine source code (ui/, Math/, input/, time/, core/)
├── include/        third-party headers (SDL3, GLAD)
├── lib/            third-party import library (libSDL3.dll.a)
├── dll/SDL3/       SDL3.dll
├── IDE/            (reserved for the future editor)
└── Makefile        builds the engine, the static library and the SDK
```

## Contributing

Bug reports, ideas and pull requests are welcome. Because the engine is pre-alpha, please open an issue to talk about a larger change before writing it.

## License

See [LICENSE](LICENSE). GGEngine uses [SDL3](https://libsdl.org) and [GLAD](https://github.com/Dav1dde/glad), which have their own licenses.
