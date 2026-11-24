#include "time/Time.hpp"

#include <SDL3/SDL.h>

namespace Timing {

namespace {
    Uint64 s_last = 0;
    bool   s_started = false;

    float  s_delta = 0.0f;
    float  s_unscaledDelta = 0.0f;
    float  s_timeScale = 1.0f;
    double s_elapsed = 0.0;
    double s_unscaledElapsed = 0.0;

    float  s_fps = 0.0f;
    int    s_frames = 0;
    float  s_fpsTimer = 0.0f;
}

void Time::update()
{
    Uint64 now = SDL_GetTicksNS();
    if (!s_started) {          // first frame: delta is 0
        s_last = now;
        s_started = true;
    }

    float rawDt = (now - s_last) / 1000000000.0f;   // nanoseconds to seconds
    s_last = now;

    // FPS uses the real, unclamped time.
    ++s_frames;
    s_fpsTimer += rawDt;
    if (s_fpsTimer >= 0.5f) {
        s_fps = s_frames / s_fpsTimer;
        s_frames = 0;
        s_fpsTimer = 0.0f;
    }

    // Delta is clamped, so a stall (like dragging the window) doesn't cause a huge jump.
    float dt = rawDt > 0.1f ? 0.1f : rawDt;

    s_unscaledDelta = dt;
    s_delta = dt * s_timeScale;
    s_unscaledElapsed += dt;
    s_elapsed += s_delta;
}

float  Time::getDelta()            { return s_delta; }
float  Time::getUnscaledDelta()    { return s_unscaledDelta; }
float  Time::getFps()              { return s_fps; }
double Time::getElapsed()          { return s_elapsed; }
double Time::getUnscaledElapsed()  { return s_unscaledElapsed; }

void Time::setTimeScale(float scale) { s_timeScale = scale < 0.0f ? 0.0f : scale; }
float Time::getTimeScale()           { return s_timeScale; }

} // namespace Timing