#pragma once

#include "ShaderPipeline.h"

class ComputeShader {
public:
	ComputeShader() = default;
	ComputeShader(const std::string & filepath);
	ComputeShader(ComputeShader&& o) noexcept;
	~ComputeShader() = default;
	void operator=(ComputeShader&& o);
	void setSrcCode(const std::string& sourceCode);

	void compute(unsigned int nbComputations);
	inline ShaderPipeline& get() { return m_shader; }

private:
	ShaderPipeline m_shader;
};