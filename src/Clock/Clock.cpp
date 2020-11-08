#include "Clock.h"

void Clock::play() {
    assert(!m_bPlaying);
    m_bPlaying = true;
}

void Clock::pause() {
    assert(m_bPlaying);
    m_bPlaying = false;
}

void Clock::togglePlayPause() {
    if (m_bPlaying)
        pause();
    else
        play();
}