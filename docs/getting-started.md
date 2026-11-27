# Getting started

This guide takes you from nothing to a game running on your screen. No engine source code is needed: you use the **GGEngine SDK**, a folder with headers and a static library, the same way you would use SDL.

- [1. Requirements](#1-requirements)
- [2. Get the SDK](#2-get-the-sdk)
- [3. Create your game project](#3-create-your-game-project)
- [4. Build and run](#4-build-and-run)
- [5. Core concepts](#5-core-concepts)
- [6. More examples](#6-more-examples)
- [7. Sharing your game](#7-sharing-your-game)
- [Building the SDK from source](#building-the-sdk-from-source)
- [Troubleshooting](#troubleshooting)

## 1. Requirements

- **Windows.** GGEngine was developed on Windows 7. Other systems are not supported yet.
- **MinGW-w64 `g++`** with C++17 support, and **GNU make** (`mingw32-make` on a normal MinGW install).
- A **graphics driver with OpenGL 2.1**. Any normal GPU driver has it. Virtual machines without 3D acceleration, or computers using only Microsoft's basic display driver, do not.

## 2. Get the SDK

The SDK is one folder:

```
GGEngine-SDK/
├── include/GGEngine/    headers (use  #include <GGEngine/GGEngine.hpp>)
├── lib/                 libGGEngine.a, libSDL3.dll.a
└── bin/                 SDL3.dll
```

Download it from the project's Releases page, or [build it yourself](#building-the-sdk-from-source).

## 3. Create your game project

Make a folder for your game and copy the SDK into it:

```
MyGame/
├── GGEngine-SDK/      the folder from step 2
├── Makefile           (below)
└── src/
    └── main.cpp       (below)
```

**`src/main.cpp`**

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

**`Makefile`**

The indented lines must start with a real **Tab** character, not spaces.

```makefile
# ===== MyGame Makefile (uses the GGEngine SDK) =====
# make / make run / make clean

SDK  := GGEngine-SDK
NAME := MyGame

CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -I$(SDK)/include -MMD -MP
LIBS     := -L$(SDK)/lib -lGGEngine -lSDL3 -static-libgcc -static-libstdc++
OBJDIR   := bin/obj
TARGET   := bin/$(NAME).exe

# ---- Shell commands: plain Windows cmd vs. Unix-style shell ----
ifeq ($(OS),Windows_NT)
  ifeq ($(shell echo %OS%),Windows_NT)
    CMD_SHELL := 1
  endif
endif

ifdef CMD_SHELL
  fixpath = $(subst /,\,$1)
  MKDIR   = if not exist "$(call fixpath,$1)" mkdir "$(call fixpath,$1)"
  CP      = copy /Y "$(call fixpath,$1)" "$(call fixpath,$2)" >nul
  RMDIR   = if exist "$(call fixpath,$1)" rmdir /S /Q "$(call fixpath,$1)"
  RUNCMD  = $(call fixpath,$(TARGET))
else
  MKDIR   = mkdir -p "$1"
  CP      = cp "$1" "$2"
  RMDIR   = rm -rf "$1"
  RUNCMD  = ./$(TARGET)
endif

SRC := $(wildcard src/*.cpp)
OBJ := $(patsubst src/%.cpp,$(OBJDIR)/%.o,$(SRC))

all: $(TARGET) bin/SDL3.dll

$(TARGET): $(OBJ) $(SDK)/lib/libGGEngine.a
	@$(call MKDIR,bin)
	$(CXX) $(OBJ) -o $@ $(LIBS)

$(OBJDIR)/%.o: src/%.cpp
	@$(call MKDIR,$(OBJDIR))
	$(CXX) $(CXXFLAGS) -c $< -o $@

bin/SDL3.dll: $(SDK)/bin/SDL3.dll
	@$(call MKDIR,bin)
	@$(call CP,$<,$@)

run: all
	$(RUNCMD)

clean:
	@$(call RMDIR,bin)

-include $(OBJ:.o=.d)

.PHONY: all run clean
```

Prefer no Makefile? One command builds the same game:

```
g++ -std=c++17 -IGGEngine-SDK/include src/main.cpp -LGGEngine-SDK/lib -lGGEngine -lSDL3 -static-libgcc -static-libstdc++ -o MyGame.exe
```

Then copy `GGEngine-SDK/bin/SDL3.dll` next to `MyGame.exe`.

## 4. Build and run

In the `MyGame` folder:

```
mingw32-make run
```

(Use `make` instead if your MinGW has it under that name.) A window opens with an orange square. Move it with the arrow keys. Close the window to quit.

The first line printed in the console is the OpenGL version, such as `OpenGL 2.1 ...`.

## 5. Core concepts

### The game loop

`win.run(...)` takes a function (a *lambda*) and calls it once per frame, about 60 times per second. Everything that changes while the game plays goes inside it:

```cpp
win.run([&](float dt) {
    // update things, then draw them
});
```

The `[&]` lets the lambda use the variables around it, such as `pos` and `draw`. Each frame, the engine does this for you: measure time, read input, clear the screen with the background color, call your function, show the result.

### The coordinate system

Positions are in **pixels**. **(0, 0) is the top-left** corner of the window. **x grows to the right, y grows downward**.

```
(0,0) ────────► x
  │
  │
  ▼
  y
```

Rectangles are drawn from their **top-left corner**.

### Delta time

`dt` is how many seconds the last frame took (about `0.016` at 60 FPS). Multiply every movement by it:

```cpp
pos.x += 200.0f * dt;     // moves 200 pixels per second on any computer
```

Without `dt`, `pos.x += 3` would move faster on faster computers.

### Held vs. just pressed

| Function | True when |
|---|---|
| `isKeyDown(key)` | the key is held, on every frame |
| `isKeyPressed(key)` | the key went down **this frame** (once per press) |
| `isKeyReleased(key)` | the key came up **this frame** |

Use `isKeyDown` for movement, and `isKeyPressed` for one-time actions like jumping or shooting. The mouse works the same way (`isButtonDown`, `isButtonPressed`, `isButtonReleased`).

### Order inside a frame

Clearing is done for you (that is what `setBackground` sets). Everything you draw goes on top of it, and **later draws cover earlier ones**:

```cpp
draw.DrawRect(Vector::Vec2D(0, 0), 100, 100, UI::RGB(255, 0, 0));    // drawn first (behind)
draw.DrawRect(Vector::Vec2D(50, 50), 100, 100, UI::RGB(0, 0, 255));  // drawn second (on top)
```

If you call `draw.ClearScreen(...)` yourself, do it **first**, because it erases everything drawn before it.

### Colors

`UI::RGB(r, g, b)` and `UI::RGBA(r, g, b, a)` take values from **0 to 255**. In `RGBA`, alpha 255 is fully visible and 0 is invisible:

```cpp
UI::RGB(255, 128, 0)         // orange
UI::RGBA(0, 255, 0, 128)     // half-transparent green
```

Each value is stored in one byte, so 300 wraps around to 44. Stay between 0 and 255.

## 6. More examples

### Smooth diagonal movement

Holding two arrow keys moves you faster diagonally if you add to `x` and `y` separately. Build a direction and normalize it:

```cpp
Vector::Vec2D dir;                                     // starts at (0, 0)
if (kb.isKeyDown(Input::Keyboard::Left))  dir.x -= 1.0f;
if (kb.isKeyDown(Input::Keyboard::Right)) dir.x += 1.0f;
if (kb.isKeyDown(Input::Keyboard::Up))    dir.y -= 1.0f;
if (kb.isKeyDown(Input::Keyboard::Down))  dir.y += 1.0f;

pos += dir.normalized() * speed * dt;                  // same speed in every direction
```

### Mouse: drag a box and click to change its color

```cpp
#include <GGEngine/GGEngine.hpp>

int main() {
    UI::Window win("Mouse", 800, 600);
    UI::DrawComponent draw(win);
    Input::Mouse mouse;

    win.setBackground(UI::RGB(30, 30, 40));

    Vector::Vec2D boxPos(300.0f, 250.0f);
    UI::RGB color(255, 128, 0);

    win.run([&](float) {
        if (mouse.isButtonDown(Input::Mouse::Left))
            boxPos = mouse.getPosition() - Vector::Vec2D(50.0f, 40.0f);   // box centered on the cursor

        if (mouse.isButtonPressed(Input::Mouse::Right)) {
            color = UI::RGB(Math::Random::nextInt(0, 255),
                            Math::Random::nextInt(0, 255),
                            Math::Random::nextInt(0, 255));
        }

        draw.DrawRect(boxPos, 100, 80, color);
    });
}
```

### Collision

```cpp
Objects::Object2D player(UI::RGB(255, 0, 0), Vector::Vec2D(100, 100), Vector::Vec2D(50, 50));
Objects::Object2D wall  (UI::RGB(0, 0, 255), Vector::Vec2D(300, 100), Vector::Vec2D(80, 200));

win.run([&](float) {
    player.Pos = mouse.getPosition();                       // the player follows the mouse

    bool hit = Collision::Collide2D(player, wall);          // do the two boxes overlap?

    draw.DrawRect(wall.Pos, wall.Size.x, wall.Size.y, wall.Color);
    draw.DrawRect(player.Pos, player.Size.x, player.Size.y,
                  hit ? UI::RGB(255, 255, 0) : player.Color);
});
```

### Timers and the FPS counter

```cpp
Timing::Timer secondTimer(1.0f);     // finishes after 1 second
secondTimer.start();

win.run([&](float dt) {
    if (secondTimer.isFinished()) {
        win.setTitle("FPS: " + std::to_string((int)Timing::Time::getFps()));
        secondTimer.start();         // start again for the next second
    }
});
```

(Add `#include <string>` for `std::to_string`.)

## 7. Sharing your game

Give people the contents of your game's `bin/` folder:

```
MyGame.exe
SDL3.dll
```

Because the engine and the C++ runtime are linked into the `.exe`, the other computer does not need MinGW. It does need a graphics driver with OpenGL 2.1. If it complains about `libwinpthread-1.dll`, copy that file from your MinGW `bin` folder next to the `.exe`.

## Building the SDK from source

You only need this if you are working on the engine itself, or if there is no ready-made SDK to download.

Requirements are the same as above. The repository includes the SDL3 and GLAD files it needs (`include/`, `lib/`, `dll/`).

```
git clone <repository-url>
cd GGEngine
mingw32-make dist
```

This builds the engine and creates the `dist/` folder, which is the SDK from step 2. Copy it into your game as `GGEngine-SDK`.

Other useful commands:

| Command | What it does |
|---|---|
| `make` | builds the demo game in `src/Main.cpp` |
| `make run` | builds and runs the demo |
| `make lib` | builds only the static library, `bin/lib/libGGEngine.a` |
| `make dist` | builds the SDK folder `dist/` |
| `make clean` | deletes `bin/` |
| `make RELEASE=1` | optimized build (add it to any command) |

## Troubleshooting

**`Makefile: *** missing separator`**
The indented lines in the Makefile must start with a Tab, not spaces. Retype the indentation as a Tab.

**`make` is not recognized**
On MinGW, the program is called `mingw32-make`. Use that name, or copy `mingw32-make.exe` to `make.exe`.

**The game won't start: "SDL3.dll was not found"**
`SDL3.dll` must be in the same folder as your `.exe`. The Makefile above copies it into `bin/`. If you built by hand, copy `GGEngine-SDK/bin/SDL3.dll` yourself.

**The game won't start: a missing `libstdc++-6.dll`, `libgcc_s_...dll` or `libwinpthread-1.dll`**
Link with `-static-libgcc -static-libstdc++` (the Makefile above does), or copy those files from MinGW's `bin` folder next to the `.exe`.

**`undefined reference to ...` while linking**
The libraries are in the wrong order or missing. Your files must come first, then `-lGGEngine`, then `-lSDL3`.

**`fatal error: GGEngine/GGEngine.hpp: No such file or directory`**
The compiler can't find the SDK headers. Check that `-IGGEngine-SDK/include` is in the compile command and that the folder `GGEngine-SDK/include/GGEngine/` exists.

**Strange errors mentioning `std::string` or `std::function` from the library**
The SDK and your game must be built with the **same MinGW/g++ version**. A static C++ library is not compatible between different compilers.

**The window opens but is black, glitchy, or the console says `OpenGL 1.1`**
Your computer is using software OpenGL. Install the graphics driver from your GPU maker (NVIDIA, AMD or Intel). On a virtual machine, turn on 3D acceleration.

**The window never opens, and the console shows an SDL error**
The first line of the error tells you why. The most common causes are a missing driver (see above) or a missing `SDL3.dll`.

**Squiggly red lines in VS Code, but `make` works**
The editor does not know where the headers are. Run **C/C++: Edit Configurations (UI)** in VS Code, add `${workspaceFolder}/GGEngine-SDK/include` to the include path, set the compiler path to your MinGW `g++.exe`, and set the C++ standard to `c++17`.

Still stuck? Open an issue on the project's repository and paste the first error message.
