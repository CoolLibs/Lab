#include "Time.h"

#include "Clock/Clock_Realtime.h"

std::unique_ptr<Clock> Time::s_clock = nullptr;

void Time::Initialize() {
    s_clock = std::make_unique<Clock_Realtime>();
}

void Time::Update() {
    s_clock->update();
}

float Time::deltaTime() {
    return s_clock->deltaTime();
}

float Time::time() {
    return s_clock->time();
}

void Time::setTime(float t) {
    s_clock->setTime(t);
}

void Time::ImGui() {
    float t = time();
    if (ImGui::DragFloat("seconds", &t)) {
        setTime(t);
    }
}