# API reference

Everything in GGEngine is available through one include:

```cpp
#include <GGEngine/GGEngine.hpp>
```

> **Pre-alpha:** names and signatures may change between versions. The `DrawComponent` functions are the most likely to be renamed (to camelCase, like the rest of the engine).

## Overview

| Namespace | Class | Purpose |
|---|---|---|
| `UI` | [`Window`](#uiwindow) | The game window and the game loop |
| `UI` | [`DrawComponent`](#uidrawcomponent) | Drawing pixels, rectangles and triangles |
| `UI` | [`RGB`, `RGBA`](#uirgb-and-uirgba) | Colors |
| `Vector` | [`Vec2D`, `Vec3D`](#vectorvec2d-and-vectorvec3d) | 2D and 3D vectors and vector math |
| `Math` | [`Random`](#mathrandom) | Random numbers |
| `Input` | [`Keyboard`](#inputkeyboard) | Keyboard state |
| `Input` | [`Mouse`](#inputmouse) | Mouse state |
| `Timing` | [`Time`](#timingtime) | Delta time, FPS, elapsed time, time scale |
| `Timing` | [`Timer`](#timingtimer) | Countdown / stopwatch |
| `Objects` | [`Object2D`, `Object3D`](#objectsobject2d-and-objectsobject3d) | Boxes with a position, size and color |
| `Collision` | [`Collide2D`, `Collide3D`](#collision) | Box overlap checks |

**Conventions**

- Positions and sizes are in **pixels**. **(0, 0) is the top-left** of the window, **x grows right, y grows down**.
- Colors use **0 to 255** per channel.
- Time is in **seconds**.
- Angles are not used yet.

---

## `UI::Window`

The game window. Creating a `Window` opens it and sets up OpenGL 2.1. Create it **before** anything that draws.

```cpp
UI::Window win("My Game", 800, 600);
```

### Constructor

| Signature | Description |
|---|---|
| `Window(const std::string& title = "GGEngine", int width = 1280, int height = 720)` | Opens the window. If it fails, the error is printed to the console and `isOpen()` returns `false`. |

A `Window` cannot be copied.

### Game loop

| Function | Description |
|---|---|
| `void run(const std::function<void(float)>& frame)` | Runs the game loop until the window is closed. Each frame it measures time, reads input, clears the screen with the background color, calls `frame(dt)`, and shows the result. `dt` is the delta time in seconds. |
| `bool isOpen() const` | `false` once the window has been closed (or failed to open). |
| `void close()` | Closes the window and ends `run()` after the current frame. |
| `void setBackground(const RGB& color)` | The color the screen is cleared with at the start of every frame. Default: black. |

```cpp
win.setBackground(UI::RGB(30, 30, 40));

win.run([&](float dt) {
    if (kb.isKeyPressed(Input::Keyboard::Escape)) win.close();
});
```

Advanced functions: `pollEvents()`, `clear(r, g, b, a)` and `swapBuffers()` are public, but `run()` already calls them for you. Call them yourself only if you write your own loop.

### Title

| Function | Description |
|---|---|
| `void setTitle(const std::string& title)` | Changes the title bar text. |
| `std::string getTitle() const` | Returns the title. |

### Size

| Function | Description |
|---|---|
| `void setSize(int width, int height)` | Resizes the window. |
| `void setMinSize(int width, int height)` | The smallest size the user can resize to. |
| `int getWidth() const` | Current width in pixels. |
| `int getHeight() const` | Current height in pixels. |

### Position

| Function | Description |
|---|---|
| `void setPosition(int x, int y)` | Moves the window on the screen. |
| `void center()` | Centers the window on the screen. |
| `int getX() const`, `int getY() const` | Window position on the screen. |

### Behavior

| Function | Description |
|---|---|
| `void setResizable(bool resizable)` | Allow or forbid resizing by the user. |
| `void setFullscreen(bool fullscreen)` | Enter or leave fullscreen. |
| `bool isFullscreen() const` | Whether the window is fullscreen. |
| `void setVisible(bool visible)` | Show or hide the window. |
| `void setVSync(bool enabled)` | VSync is on by default (it limits the frame rate to your monitor's refresh rate). Pass `false` to remove the limit. |

---

## `UI::DrawComponent`

Draws shapes into a window. Create it **after** the window, and keep the window alive as long as the `DrawComponent` exists.

```cpp
UI::Window win("Game", 800, 600);
UI::DrawComponent draw(win);
```

Call the draw functions **inside** your `run` function. Later calls are drawn on top of earlier ones.

### Constructor

| Signature | Description |
|---|---|
| `explicit DrawComponent(const Window& window)` | Prepares drawing for this window and turns on transparency (alpha blending). |

### Functions

Every drawing function has two versions: one taking an `RGB` and one taking an `RGBA`. Only `RGBA` can be transparent.

| Function | Description |
|---|---|
| `void ClearScreen()` | Clears the whole screen to black. |
| `void ClearScreen(const RGB& color)` / `(const RGBA& color)` | Clears the whole screen to a color. This erases everything already drawn this frame, so call it **first**. (`setBackground` does this for you automatically.) |
| `void DrawPixel(const Vector::Vec2D& position, color)` | Draws one pixel. |
| `void DrawRect(const Vector::Vec2D& position, float width, float height, color)` | Draws a filled rectangle. `position` is its **top-left corner**. |
| `void DrawTriangle(const Vector::Vec3D& a, const Vector::Vec3D& b, const Vector::Vec3D& c, color)` | Draws a filled triangle with corners `a`, `b`, `c`. Only `x` and `y` are used for now. |

```cpp
draw.DrawRect(Vector::Vec2D(100, 100), 200, 150, UI::RGB(255, 128, 0));

draw.DrawTriangle(Vector::Vec3D(400, 100, 0),
                  Vector::Vec3D(500, 300, 0),
                  Vector::Vec3D(300, 300, 0),
                  UI::RGBA(0, 255, 0, 128));      // half-transparent green
```

Drawing is simple and meant for small scenes. Drawing thousands of single pixels will be slow, so use `DrawRect` for large areas.

---

## `UI::RGB` and `UI::RGBA`

Colors with channels from 0 to 255 (stored as `uint8_t`).

```cpp
UI::RGB orange(255, 128, 0);
UI::RGBA glass(0, 100, 255, 90);      // translucent blue
UI::RGBA solid(255, 0, 0);            // alpha defaults to 255 (fully visible)
```

| Member / function | Description |
|---|---|
| `r`, `g`, `b` | Red, green, blue (0 to 255). |
| `a` (`RGBA` only) | Alpha: 255 is fully visible, 0 is invisible. |
| `RGB()` / `RGBA()` | Black (`RGBA` is fully visible). |
| `RGB(r, g, b)` / `RGBA(r, g, b, a = 255)` | Create a color. |
| `GLRGB ToGL() const` / `GLRGBA ToGL() const` | Converts to OpenGL's format: floats from 0.0 to 1.0 in the members `r`, `g`, `b` (and `a`). |

Values outside 0 to 255 wrap around (300 becomes 44), so stay in range.

---

## `Vector::Vec2D` and `Vector::Vec3D`

Vectors of `float` values. `Vec2D` has `x` and `y`. `Vec3D` has `x`, `y` and `z`. Members start at `0`.

```cpp
Vector::Vec2D a(3.0f, 4.0f);
Vector::Vec2D b;                 // (0, 0)
b.x = 10;
```

### Operators

| Expression | Result |
|---|---|
| `a + b`, `a - b` | Add and subtract vectors. |
| `a * 2.0f`, `2.0f * a`, `a / 2.0f` | Scale a vector. |
| `-a` | Opposite vector. |
| `a += b`, `a -= b`, `a *= s`, `a /= s` | Change `a` in place. |
| `a == b`, `a != b` | Exact comparison. Floats rarely stay exactly equal after math, so use this for things like "is it the zero vector". |

### Functions

| Function | Description |
|---|---|
| `float length() const` | Length of the vector. `Vec2D(3, 4).length()` is `5`. |
| `float lengthSquared() const` | Length squared. Faster, good for comparing distances. |
| `float dot(const Vec& other) const` | Dot product. |
| `float distanceTo(const Vec& other) const` | Distance between two points. |
| `Vec normalized() const` | A copy with length 1 (the zero vector stays zero). |
| `void normalize()` | Normalizes this vector in place. |
| `Vec3D cross(const Vec3D& other) const` | Cross product (`Vec3D` only). |

```cpp
Vector::Vec2D dir(1.0f, 1.0f);
pos += dir.normalized() * speed * dt;     // same speed in every direction
```

---

## `Math::Random`

Static functions. The generator is seeded automatically each run.

| Function | Description |
|---|---|
| `static int nextInt(int min, int max)` | A whole number from `min` to `max`, **both included**. |
| `static float nextFloat()` | A number from 0.0 up to (not including) 1.0. |
| `static float nextFloat(float min, float max)` | A number from `min` up to (not including) `max`. |
| `static bool nextBool()` | `true` or `false`, 50/50. |
| `static void setSeed(unsigned int seed)` | The same seed always gives the same sequence. Useful for testing. |

```cpp
int dice = Math::Random::nextInt(1, 6);
```

---

## `Input::Keyboard`

Keyboard state. All functions are `static`, so you can call them as `Input::Keyboard::isKeyDown(...)`, or create an object and call them on it (`Input::Keyboard kb; kb.isKeyDown(...)`). Both work the same.

Keys are identified by their **physical position** on the keyboard, so WASD is in the same place on every keyboard layout.

| Function | True when |
|---|---|
| `static bool isKeyDown(Key key)` | The key is held, every frame. |
| `static bool isKeyPressed(Key key)` | The key went down this frame (once per press). |
| `static bool isKeyReleased(Key key)` | The key came up this frame. |

### Keys

Written as `Input::Keyboard::<Name>`:

| Group | Names |
|---|---|
| Letters | `A` to `Z` |
| Digits | `Num0` to `Num9` |
| Arrows | `Left`, `Right`, `Up`, `Down` |
| Common | `Space`, `Enter`, `Escape`, `Tab`, `Backspace` |
| Modifiers | `LeftShift`, `RightShift`, `LeftCtrl`, `RightCtrl`, `LeftAlt`, `RightAlt` |
| Function keys | `F1` to `F12` |

```cpp
Input::Keyboard kb;

if (kb.isKeyDown(Input::Keyboard::Left)) pos.x -= speed * dt;
if (kb.isKeyPressed(Input::Keyboard::Space)) jump();
```

---

## `Input::Mouse`

Mouse state. Like `Keyboard`, all functions are `static` and also work on an object.

### Buttons

| Function | True when |
|---|---|
| `static bool isButtonDown(Button button)` | The button is held, every frame. |
| `static bool isButtonPressed(Button button)` | The button went down this frame. |
| `static bool isButtonReleased(Button button)` | The button came up this frame. |

Buttons: `Input::Mouse::Left`, `Middle`, `Right`, `Back`, `Forward`.

### Position, movement and wheel

| Function | Description |
|---|---|
| `static float getX()`, `static float getY()` | Cursor position inside the window, in pixels. |
| `static Vector::Vec2D getPosition()` | Cursor position as a vector. |
| `static float getDeltaX()`, `static float getDeltaY()` | How far the cursor moved this frame. |
| `static float getScrollX()`, `static float getScrollY()` | Wheel movement this frame. Positive Y means scrolled up. |
| `static void setCursorVisible(bool visible)` | Show or hide the system cursor. |

```cpp
Input::Mouse mouse;

if (mouse.isButtonPressed(Input::Mouse::Left))
    spawnAt(mouse.getPosition());

scale += mouse.getScrollY() * 0.1f;
```

On a display with scaling above 100%, mouse positions are in window units, which can differ from pixels.

---

## `Timing::Time`

Static functions for the clock. The engine updates it once per frame.

| Function | Description |
|---|---|
| `static float getDelta()` | Seconds since the last frame, multiplied by the time scale. This is the same value your `run` function receives as `dt`. |
| `static float getUnscaledDelta()` | Seconds since the last frame, ignoring the time scale. |
| `static float getFps()` | Frames per second, refreshed twice a second. |
| `static double getElapsed()` | Game time since the start, in seconds (follows the time scale). |
| `static double getUnscaledElapsed()` | Real time since the start, in seconds. |
| `static void setTimeScale(float scale)` | `1` is normal speed, `0.5` is slow motion, `0` pauses everything that uses delta time or `Timer`. |
| `static float getTimeScale()` | The current time scale. |

Delta time is capped at 0.1 seconds, so a stall (such as dragging the window) does not make objects jump.

---

## `Timing::Timer`

A countdown or stopwatch that follows game time, so it slows down or pauses with the time scale.

```cpp
Timing::Timer shootCooldown(0.5f);     // 0.5 seconds
shootCooldown.start();
```

| Function | Description |
|---|---|
| `explicit Timer(float durationSeconds = 1.0f)` | Creates a timer (not started yet). |
| `void start()` | Starts from zero. Also restarts a running or finished timer. |
| `void stop()` | Stops and resets. |
| `void pause()` / `void resume()` | Pause and continue counting. |
| `bool isRunning() const` | Whether it is counting right now. |
| `bool isFinished() const` | `true` once the duration has been reached (and stays true until `start()` or `stop()`). |
| `float getElapsed() const` | Seconds counted so far. |
| `float getRemaining() const` | Seconds left (never below 0). |
| `void setDuration(float seconds)` / `float getDuration() const` | Change or read the duration. |

```cpp
if (shootCooldown.isFinished() && kb.isKeyPressed(Input::Keyboard::Space)) {
    shoot();
    shootCooldown.start();
}
```

---

## `Objects::Object2D` and `Objects::Object3D`

Simple boxes with a position, a size and a color. They are what `Collision` works with.

```cpp
Objects::Object2D player(UI::RGB(255, 0, 0),         // color
                         Vector::Vec2D(100, 100),    // position (top-left corner)
                         Vector::Vec2D(50, 50));     // size
```

| Member | Description |
|---|---|
| `Vector::Vec2D Pos` (`Vec3D` in `Object3D`) | Position. For `Object2D` it is the top-left corner, like `DrawRect`. For `Object3D` it is the corner with the smallest x, y and z. |
| `Vector::Vec2D Size` (`Vec3D` in `Object3D`) | Width and height (and depth in 3D). |
| `UI::RGB Color` | The object's color. |
| Constructor `(UI::RGB color, Vec position, Vec size)` | Creates the object. |

`Objects` do not draw themselves yet. Draw them with `DrawRect(obj.Pos, obj.Size.x, obj.Size.y, obj.Color)`.

---

## `Collision`

Checks whether two boxes overlap.

| Function | Description |
|---|---|
| `bool Collision::Collide2D(const Objects::Object2D& a, const Objects::Object2D& b)` | `true` if the two rectangles overlap. |
| `bool Collision::Collide3D(const Objects::Object3D& a, const Objects::Object3D& b)` | `true` if the two boxes overlap. |

Boxes that only **touch** at an edge do not count as colliding.

```cpp
if (Collision::Collide2D(player, enemy)) {
    gameOver = true;
}
```
