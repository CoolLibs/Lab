#include "Time.h"

#include "Clock/Clock_Realtime.h"
#include <Cool/ImGui/ImGui.h>
#include "Constants/Textures.h"

std::unique_ptr<Clock> Time::m_clock = nullptr;

void Time::Initialize() {
    m_clock = std::make_unique<Clock_Realtime>();
}

void Time::Update() {
    m_clock->update();
}

float Time::deltaTime() {
    return m_clock->deltaTime();
}

float Time::time() {
    return m_clock->time();
}

void Time::setTime(float t) {
    m_clock->setTime(t);
}

void Time::ImGui() {
    if (CoolImGui::ButtonWithIcon(m_clock->isPlaying() ? Textures::Pause() : Textures::Play())) {
        m_clock->togglePlayPause();
    }
    ImGui::SameLine();
    float t = time();
    if (ImGui::DragFloat("seconds", &t, 0.5f, 0.f, 0.f, "%.2f")) {
        setTime(t);
    }
}