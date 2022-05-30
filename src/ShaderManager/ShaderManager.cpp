#include "ShaderManager.h"
#include <Cool/Camera/CameraShaderU.h>

void ShaderManager::setup_for_rendering(const Cool::Camera& camera, float time)
{
}

void ShaderManager::render()
{
    _fullscreen_pipeline.draw();
}