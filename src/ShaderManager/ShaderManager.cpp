#include "ShaderManager.h"

void ShaderManager::setup_for_rendering(const Cool::Camera& camera, float focal_length, float aspect_ratio, float time)
{
    _fullscreen_pipeline.shader().bind();
    _fullscreen_pipeline.shader().set_uniform("_aspect_ratio", aspect_ratio);
    _fullscreen_pipeline.shader().set_uniform("_camera_right_axis", camera.right_axis());
    _fullscreen_pipeline.shader().set_uniform("_camera_up_axis", camera.up_axis());
    _fullscreen_pipeline.shader().set_uniform("_camera_front_axis", camera.front_axis());
    _fullscreen_pipeline.shader().set_uniform("_camera_position", camera.position());
    _fullscreen_pipeline.shader().set_uniform("_focal_length", focal_length);
    _fullscreen_pipeline.shader().set_uniform("_time", time);
}

void ShaderManager::render()
{
    _fullscreen_pipeline.draw();
}