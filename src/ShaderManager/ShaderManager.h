#pragma once

#include <Cool/OpenGL/Shader.h>

using namespace Cool;
namespace Cool {
	class Camera;
}

class ShaderManager {
public:
	virtual void setup_for_rendering(const Camera& camera, float focal_length);

	virtual void update() = 0;
	virtual void ImGui_window() = 0;
	/// <summary>
	/// 
	/// </summary>
	/// <returns>true iff we currently have a valid shader that can be used for rendering</returns>
	virtual bool is_valid() const = 0;

protected:
	Shader _shader;
};