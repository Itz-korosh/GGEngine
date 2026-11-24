#include "ui/Window.hpp"

#include <SDL3/SDL.h>
#include <glad/glad.h>   // GLAD v1; must come before any other OpenGL header
#include <iostream>

#include "input/Keyboard.hpp"
#include "input/Mouse.hpp"
#include "time/Time.hpp"

namespace UI {

Window::Window(const std::string& title, int width, int height)
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
        return;
    }

    // Ask for an OpenGL 2.1 context (must be set before creating the window).
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    m_window = SDL_CreateWindow(title.c_str(), width, height,
                                SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!m_window) {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << "\n";
        return;
    }

    SDL_GLContext context = SDL_GL_CreateContext(m_window);
    if (!context) {
        std::cerr << "SDL_GL_CreateContext failed: " << SDL_GetError() << "\n";
        return;
    }
    m_context = context;

    // Load the OpenGL functions (needs a current context).
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        std::cerr << "Failed to load OpenGL functions with GLAD\n";
        return;
    }

    SDL_GL_SetSwapInterval(1);   // VSync on

    std::cout << "OpenGL " << glGetString(GL_VERSION) << "\n";
    m_open = true;
}

Window::~Window()
{
    if (m_context) SDL_GL_DestroyContext((SDL_GLContext)m_context);
    if (m_window)  SDL_DestroyWindow(m_window);
    SDL_Quit();
}

// ---- Game loop ----

void Window::run(const std::function<void(float)>& frame)
{
    while (m_open) {
        Timing::Time::update();   // measure this frame
        pollEvents();
        clear(m_background.r, m_background.g, m_background.b, m_background.a);
        frame(Timing::Time::getDelta());
        swapBuffers();
    }
}

void Window::pollEvents()
{
    Input::Keyboard::update();
    Input::Mouse::update();

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_EVENT_QUIT:
            case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                m_open = false;
                break;

            case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
                glViewport(0, 0, event.window.data1, event.window.data2);
                break;
        }
    }
}

void Window::clear(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Window::swapBuffers()
{
    if (m_window) SDL_GL_SwapWindow(m_window);
}

// ---- Background ----

void Window::setBackground(const RGB& color)
{
    GLRGB c = color.ToGL();
    m_background = GLRGBA{ c.r, c.g, c.b, 1.0f };
}

// ---- Title ----

void Window::setTitle(const std::string& title)
{
    if (m_window) SDL_SetWindowTitle(m_window, title.c_str());
}

std::string Window::getTitle() const
{
    return m_window ? SDL_GetWindowTitle(m_window) : "";
}

// ---- Size ----

void Window::setSize(int width, int height)
{
    if (m_window) SDL_SetWindowSize(m_window, width, height);
}

void Window::setMinSize(int width, int height)
{
    if (m_window) SDL_SetWindowMinimumSize(m_window, width, height);
}

int Window::getWidth() const
{
    int w = 0, h = 0;
    if (m_window) SDL_GetWindowSizeInPixels(m_window, &w, &h);
    return w;
}

int Window::getHeight() const
{
    int w = 0, h = 0;
    if (m_window) SDL_GetWindowSizeInPixels(m_window, &w, &h);
    return h;
}

// ---- Position ----

void Window::setPosition(int x, int y)
{
    if (m_window) SDL_SetWindowPosition(m_window, x, y);
}

void Window::center()
{
    if (m_window) SDL_SetWindowPosition(m_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}

int Window::getX() const
{
    int x = 0, y = 0;
    if (m_window) SDL_GetWindowPosition(m_window, &x, &y);
    return x;
}

int Window::getY() const
{
    int x = 0, y = 0;
    if (m_window) SDL_GetWindowPosition(m_window, &x, &y);
    return y;
}

// ---- Behavior ----

void Window::setResizable(bool resizable)
{
    if (m_window) SDL_SetWindowResizable(m_window, resizable);
}

void Window::setFullscreen(bool fullscreen)
{
    if (m_window) SDL_SetWindowFullscreen(m_window, fullscreen);
}

bool Window::isFullscreen() const
{
    return m_window && (SDL_GetWindowFlags(m_window) & SDL_WINDOW_FULLSCREEN) != 0;
}

void Window::setVisible(bool visible)
{
    if (!m_window) return;
    if (visible) SDL_ShowWindow(m_window);
    else         SDL_HideWindow(m_window);
}

void Window::setVSync(bool enabled)
{
    SDL_GL_SetSwapInterval(enabled ? 1 : 0);
}

} // namespace UI