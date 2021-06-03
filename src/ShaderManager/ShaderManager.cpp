#include "ShaderManager.h"

#include <Cool/Camera/Camera.h>
#include <Cool/Time/Time.h>
#include <Cool/App/RenderState.h>

void ShaderManager::setup_for_rendering(const Camera& camera, float focal_length) {
	_shader.bind();
	_shader.set_uniform("uAspectRatio", RenderState::Size().aspectRatio());
	_shader.set_uniform("uCamX", camera.right_axis());
	_shader.set_uniform("uCamY", camera.up_axis());
	_shader.set_uniform("uCamZ", camera.front_axis());
	_shader.set_uniform("uCamPos", camera.position());
	_shader.set_uniform("uFocalLength", focal_length);
	_shader.set_uniform("uTime", Time::time());
}