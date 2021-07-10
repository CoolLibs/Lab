#include "ShaderManager.h"

#include <Cool/Camera/Camera.h>
#include <Cool/Time/Time.h>
#include <Cool/App/RenderState.h>

void ShaderManager::setup_for_rendering(const Camera& camera, float focal_length) {
	_shader.bind();
	_shader.set_uniform("_aspect_ratio", RenderState::Size().aspectRatio());
	_shader.set_uniform("_camera_right_axis", camera.right_axis());
	_shader.set_uniform("_camera_up_axis", camera.up_axis());
	_shader.set_uniform("_camera_front_axis", camera.front_axis());
	_shader.set_uniform("_camera_position", camera.position());
	_shader.set_uniform("_focal_length", focal_length);
	_shader.set_uniform("_time", Time::time());
}