#include "Clock_Realtime.h"

float Clock_Realtime::SDLPerformanceCounter2Seconds;

void Clock_Realtime::Initialize() {
    SDLPerformanceCounter2Seconds = 1.0f / SDL_GetPerformanceFrequency();
}

Clock_Realtime::Clock_Realtime()
    : m_lastTime(SDL_GetPerformanceCounter()), m_currentTime(m_lastTime), m_offsetWithSDLTime(m_lastTime * SDLPerformanceCounter2Seconds)
{}

float Clock_Realtime::deltaTime() const {
    if (Clock::isPlaying())
        return (m_currentTime - m_lastTime) * SDLPerformanceCounter2Seconds;
    else
        return 0.f;
}

float Clock_Realtime::time() const {
	return m_currentTime * SDLPerformanceCounter2Seconds - m_offsetWithSDLTime;
}

void Clock_Realtime::setTime(float newTime) {
    m_offsetWithSDLTime += time() - newTime;
}

void Clock_Realtime::update() {
    if (Clock::isPlaying()) {
        m_lastTime = m_currentTime;
        m_currentTime = SDL_GetPerformanceCounter();
    }
}

void Clock_Realtime::play() {
    Clock::play();
    m_offsetWithSDLTime += (SDL_GetPerformanceCounter() - m_SDLTimeWhenPaused) * SDLPerformanceCounter2Seconds;
    update();
}

void Clock_Realtime::pause() {
    Clock::pause();
    m_SDLTimeWhenPaused = SDL_GetPerformanceCounter();
}