#include "ShaderManager.h"
#include <Cool/Camera/CameraShaderU.h>

void ShaderManager::setup_for_rendering(const Cool::Camera& camera, float time)
{
    if (_fullscreen_pipeline.shader().has_value()) {
        _fullscreen_pipeline.shader()->bind();
        _fullscreen_pipeline.shader()->set_uniform("_time", time);
        Cool::CameraShaderU::set_uniform(*_fullscreen_pipeline.shader(), camera);
    }
}

void ShaderManager::render()
{
    _fullscreen_pipeline.draw();
}