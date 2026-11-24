#pragma once

namespace Timing {

class Time {
public:
    // Called by Window once per frame. Game code never calls this.
    static void update();

    static float  getDelta();            // seconds since last frame (affected by time scale)
    static float  getUnscaledDelta();    // seconds since last frame (real time)
    static float  getFps();              // frames per second, refreshed twice a second
    static double getElapsed();          // game time since start (stops if time scale is 0)
    static double getUnscaledElapsed();  // real time since start

    static void  setTimeScale(float scale);   // 1 = normal, 0.5 = slow motion, 0 = paused
    static float getTimeScale();
};

} // namespace Timing