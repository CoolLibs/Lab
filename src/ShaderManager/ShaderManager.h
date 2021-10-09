#pragma once

#include <Cool/Camera/Camera.h>
#include <Cool/File/File.h>
#include <Cool/Gpu/FullscreenPipeline.h>

class ShaderManager {
public:
    virtual void setup_for_rendering(const Cool::Camera& camera, float time);
    void         render();

    virtual void update()       = 0;
    virtual void imgui_window() = 0;

protected:
    Cool::FullscreenPipeline _fullscreen_pipeline{};
};