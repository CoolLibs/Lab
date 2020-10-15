#include "Clock_Realtime.h"

float Clock_Realtime::SDLPerformanceCounter2Seconds;

void Clock_Realtime::Initialize() {
    SDLPerformanceCounter2Seconds = 1.0f / SDL_GetPerformanceFrequency();
}

Clock_Realtime::Clock_Realtime()
    : m_lastTime(SDL_GetPerformanceCounter()), m_currentTime(m_lastTime), m_offsetWithSDLTime(m_lastTime * SDLPerformanceCounter2Seconds)
{}

float Clock_Realtime::deltaTime() const {
    return (m_currentTime - m_lastTime) * SDLPerformanceCounter2Seconds;
}

float Clock_Realtime::time() const {
	return m_currentTime * SDLPerformanceCounter2Seconds - m_offsetWithSDLTime;
}

void Clock_Realtime::setTime(float newTime) {
    m_offsetWithSDLTime += time() - newTime;
}

void Clock_Realtime::update(){
    m_lastTime = m_currentTime;
    m_currentTime = SDL_GetPerformanceCounter();
}