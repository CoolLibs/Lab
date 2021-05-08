#pragma once

#include "ShaderManager.h"
#include <Cool/Params/internal/IParam.h>
#include <Cool/FileWatcher/FileWatcher.h>

class ShaderManager_FromText : public ShaderManager {
public:
	ShaderManager_FromText();

	void setup_for_rendering(const Camera& camera) override;

	inline void update() override { m_shaderWatcher.update(); }
	inline bool is_valid() const override { return m_shaderWatcher.pathIsValid(); } // TODO doesn't currently check that the compilation succeeded
	void ImGui_window() override;

	void setShaderPath(std::string_view path);

private:
	void compile_shader(const char* path);
	void parse_shader_for_params(const char* path);
	size_t find_param(std::string_view name);

private:
	FileWatcher m_shaderWatcher;
	std::vector<std::unique_ptr<Cool::Internal::IParam>> _dynamic_params;

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