#pragma once

namespace Timing {

// A countdown / stopwatch that follows game time (so it pauses when time scale is 0).
class Timer {
public:
    explicit Timer(float durationSeconds = 1.0f);

    void start();     // start from zero (also restarts)
    void stop();      // stop and reset
    void pause();
    void resume();

    bool  isRunning() const { return m_running; }
    bool  isFinished() const;      // started and the duration has been reached
    float getElapsed() const;      // seconds counted so far
    float getRemaining() const;    // seconds left (never below 0)

    void  setDuration(float seconds) { m_duration = seconds; }
    float getDuration() const { return m_duration; }

private:
    float  m_duration;
    float  m_accumulated = 0.0f;   // time counted before the current run
    double m_resumeTime = 0.0;     // game time when it last started or resumed
    bool   m_started = false;
    bool   m_running = false;
};

} // namespace Timing