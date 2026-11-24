#include "time/Timer.hpp"
#include "time/Time.hpp"

namespace Timing {

Timer::Timer(float durationSeconds)
    : m_duration(durationSeconds)
{
}

void Timer::start()
{
    m_accumulated = 0.0f;
    m_resumeTime = Time::getElapsed();
    m_started = true;
    m_running = true;
}

void Timer::stop()
{
    m_accumulated = 0.0f;
    m_started = false;
    m_running = false;
}

void Timer::pause()
{
    if (!m_running) return;
    m_accumulated += static_cast<float>(Time::getElapsed() - m_resumeTime);
    m_running = false;
}

void Timer::resume()
{
    if (!m_started || m_running) return;
    m_resumeTime = Time::getElapsed();
    m_running = true;
}

float Timer::getElapsed() const
{
    float elapsed = m_accumulated;
    if (m_running)
        elapsed += static_cast<float>(Time::getElapsed() - m_resumeTime);
    return elapsed;
}

float Timer::getRemaining() const
{
    float left = m_duration - getElapsed();
    return left > 0.0f ? left : 0.0f;
}

bool Timer::isFinished() const
{
    return m_started && getElapsed() >= m_duration;
}

} // namespace Timing