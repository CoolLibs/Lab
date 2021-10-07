#include "ShaderManager.h"

void ShaderManager::setup_for_rendering(const Cool::Camera& camera, float time)
{
    _fullscreen_pipeline.shader().bind();
    _fullscreen_pipeline.shader().set_uniform("_camera_transform", camera.transform_matrix());
    _fullscreen_pipeline.shader().set_uniform("_camera_inverse_projection", glm::inverse(camera.projection_matrix()));
    _fullscreen_pipeline.shader().set_uniform("_time", time);
}

void ShaderManager::render()
{
    _fullscreen_pipeline.draw();
}