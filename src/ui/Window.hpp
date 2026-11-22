#pragma once

#include <string>
#include <functional>

// Forward declaration, so this header doesn't need to include SDL.
struct SDL_Window;

namespace UI {

class Window {
public:
    Window(const std::string& title = "GGEngine", int width = 1280, int height = 720);
    ~Window();

    // A window owns system resources, so copying it makes no sense.
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    // ---- Game loop ----
    void run(const std::function<void()>& frame);
    void pollEvents();
    void clear(float r, float g, float b, float a);
    void swapBuffers();
    bool isOpen() const { return m_open; }

    // ---- Title ----
    void setTitle(const std::string& title);
    std::string getTitle() const;

    // ---- Size (in pixels) ----
    void setSize(int width, int height);
    void setMinSize(int width, int height);
    int getWidth() const;
    int getHeight() const;

    // ---- Position ----
    void setPosition(int x, int y);
    void center();
    int getX() const;
    int getY() const;

    // ---- Behavior ----
    void setResizable(bool resizable);
    void setFullscreen(bool fullscreen);
    bool isFullscreen() const;
    void setVisible(bool visible);
    void setVSync(bool enabled);

private:
    SDL_Window* m_window = nullptr;
    void* m_context = nullptr;   // the OpenGL context (SDL_GLContext)
    bool m_open = false;
};

} // namespace UI