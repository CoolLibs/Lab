#include "ShaderPipeline.h"

ShaderPipeline::ShaderPipeline()
	: m_programID(0)
{}

ShaderPipeline::ShaderPipeline(ShaderPipeline&& o) noexcept
	: m_shaders(std::move(o.m_shaders)), m_programID(o.m_programID)
{
	o.m_programID = 0;
}

void ShaderPipeline::operator=(ShaderPipeline&& o) {
	m_shaders = std::move(o.m_shaders);
	m_programID = o.m_programID;
	//
	o.m_programID = 0;
}

ShaderPipeline::~ShaderPipeline() {
	glDeleteProgram(m_programID);
}

void ShaderPipeline::addShader(ShaderType type, const std::string& shaderFilepath) {
	m_shaders.emplace_back(type, shaderFilepath);
}

void ShaderPipeline::addShaderBySrcCode(ShaderType type, const std::string& shaderSrc) {
	m_shaders.emplace_back(type, shaderSrc, false);
}

void ShaderPipeline::createProgram() {
	if (m_programID != 0)
		GLCall(glDeleteProgram(m_programID));
	GLCall(m_programID = glCreateProgram());
	for (Shader& shader : m_shaders) {
		shader.compile();
		GLCall(glAttachShader(m_programID, shader.getID()));
	}
	GLCall(glLinkProgram(m_programID));
	GLCall(glValidateProgram(m_programID));
	m_shaders.clear(); // delete all shaders
	//Log::separationLine();
}

void ShaderPipeline::bind() {
	glUseProgram(m_programID);
}
void ShaderPipeline::unbind() {
	glUseProgram(0);
}

int ShaderPipeline::getUniformLocation(const std::string& uniformName) {
	if (m_UniformLocationCache.find(uniformName) != m_UniformLocationCache.end())
		return m_UniformLocationCache[uniformName];
	int location = glGetUniformLocation(m_programID, uniformName.c_str());
	m_UniformLocationCache[uniformName] = location;
	return location;
}

void ShaderPipeline::setUniform1i(const std::string& uniformName, int v) {
	glUniform1i(getUniformLocation(uniformName), v);
}
void ShaderPipeline::setUniform1f(const std::string& uniformName, float v) {
	glUniform1f(getUniformLocation(uniformName), v);
}
void ShaderPipeline::setUniform2f(const std::string& uniformName, const glm::vec2& v) {
	glUniform2f(getUniformLocation(uniformName), v.x, v.y);
}
void ShaderPipeline::setUniform2f(const std::string& uniformName, const float* v) {
	glUniform2f(getUniformLocation(uniformName), v[0], v[1]);
}
void ShaderPipeline::setUniform3f(const std::string& uniformName, const glm::vec3& v) {
	glUniform3f(getUniformLocation(uniformName), v.x, v.y, v.z);
}
void ShaderPipeline::setUniform3f(const std::string& uniformName, float v1, float v2, float v3) {
	glUniform3f(getUniformLocation(uniformName), v1, v2, v3);
}