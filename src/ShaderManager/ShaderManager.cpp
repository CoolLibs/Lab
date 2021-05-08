#include "ShaderManager.h"

#include <Cool/Camera/Camera.h>
#include <Cool/Time/Time.h>
#include <Cool/App/RenderState.h>

void ShaderManager::setup_for_rendering(const Camera& camera) {
	_shader.bind();
	_shader.set_uniform("uAspectRatio", RenderState::Size().aspectRatio());
	_shader.set_uniform("uCamX", camera.xAxis());
	_shader.set_uniform("uCamY", camera.yAxis());
	_shader.set_uniform("uCamZ", camera.zAxis());
	_shader.set_uniform("uCamPos", camera.position());
	_shader.set_uniform("uFocalLength", camera.focalLength());
	_shader.set_uniform("uTime", Time::time());
}