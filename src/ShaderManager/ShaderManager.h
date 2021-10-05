#pragma once

#include <Cool/Camera/Camera.h>
#include <Cool/File/File.h>
#include <Cool/Gpu/FullscreenPipeline.h>

class ShaderManager {
public:
    virtual void setup_for_rendering(const Cool::Camera& camera, float focal_length, float aspect_ratio, float time);
    void         render();

    virtual void update()       = 0;
    virtual void ImGui_window() = 0;
    /// <summary>
    ///
    /// </summary>
    /// <returns>true iff we currently have a valid shader that can be used for rendering</returns>
    virtual bool is_valid() const = 0;

protected:
    Cool::FullscreenPipeline _fullscreen_pipeline{Cool::File::root_dir() + "/shader-examples/axes.frag"};
};