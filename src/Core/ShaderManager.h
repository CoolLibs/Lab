#pragma once

#include <Cool/FileWatcher/FileWatcher.h>
#include <Cool/OpenGL/Shader.h>

using namespace Cool;
namespace Cool {
	class Camera;
}

class ShaderManager {
public:
	ShaderManager();
	~ShaderManager() = default;

	inline void update() { m_shaderWatcher.update(); }
	/// <summary>
	/// 
	/// </summary>
	/// <returns>true iff we currently have a valid shader that can be used for rendering</returns>
	inline bool isValid() const { return m_shaderWatcher.pathIsValid(); } // TODO doesn't currently check that the compilation succeeded
	void setupForRendering(const Camera& camera);
	void ImGui();

	void setShaderPath(std::string_view path);

private:
	std::string m_currentShaderPath = "";
	Shader m_shader; // Must be declared before m_shaderWatcher because the latter compiles m_shader
	FileWatcher m_shaderWatcher;
};