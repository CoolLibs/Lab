#include "Clock_FixedTimestep.h"

Clock_FixedTimestep::Clock_FixedTimestep(float fps, float initialTime)
    : m_dt(1.0f / fps)
{
    m_framesCount = static_cast<unsigned int>(std::round(initialTime * fps));
}

float Clock_FixedTimestep::deltaTime() const {
    if (Clock::isPlaying())
        return m_dt;
    else
        return 0.f;
}

float Clock_FixedTimestep::time() const {
    return m_framesCount * m_dt;
}

void Clock_FixedTimestep::setTime(float newTime) {
    m_framesCount = static_cast<unsigned int>(round(newTime / m_dt));
}

void Clock_FixedTimestep::update() {
    if (Clock::isPlaying())
        m_framesCount++;
}