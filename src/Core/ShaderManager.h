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
	Shader m_shader;
	FileWatcher m_shaderWatcher;

private:
	//Serialization
	friend class cereal::access;
	template <class Archive>
	void save(Archive& archive) const
	{
		archive(
			cereal::make_nvp("Shader Path", m_shaderWatcher.path().string())
		);
	}
	template<class Archive>
	void load(Archive& archive)
	{
		std::string path;
		archive(
			path
		);
		setShaderPath(path);
	}
};