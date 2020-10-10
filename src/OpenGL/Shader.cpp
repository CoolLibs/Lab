#include "Shader.h"

Shader::Shader(const std::string& vertexShaderFilepath, const std::string& fragmentShaderFilepath) {
	GLCall(m_shaderId = glCreateProgram());
	unsigned int vs = ShaderHelper::compileShader(GL_VERTEX_SHADER,   ShaderHelper::parseFile(vertexShaderFilepath));
	unsigned int fs = ShaderHelper::compileShader(GL_FRAGMENT_SHADER, ShaderHelper::parseFile(fragmentShaderFilepath));

	GLCall(glAttachShader(m_shaderId, vs));
	GLCall(glAttachShader(m_shaderId, fs));
	GLCall(glLinkProgram(m_shaderId));
	GLCall(glValidateProgram(m_shaderId));

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));
}

Shader::~Shader() {
	GLCall(glDeleteProgram(m_shaderId));
}

void Shader::bind() {
	GLCall(glUseProgram(m_shaderId));
}

int Shader::getUniformLocation(const char* uniformName) {
	if (m_uniformLocationCache.find(uniformName) != m_uniformLocationCache.end()) {
		return m_uniformLocationCache[uniformName];
	}
	GLCall(int location = glGetUniformLocation(m_shaderId, uniformName));
	m_uniformLocationCache[uniformName] = location;
	return location;
}

void Shader::setUniform1i(const char* uniformName, int v) {
	GLCall(glUniform1i(getUniformLocation(uniformName), v));
}
void Shader::setUniform1f(const char* uniformName, float v) {
	GLCall(glUniform1f(getUniformLocation(uniformName), v));
}
void Shader::setUniform2f(const char* uniformName, const glm::vec2& v) {
	GLCall(glUniform2f(getUniformLocation(uniformName), v.x, v.y));
}
void Shader::setUniform3f(const char* uniformName, const glm::vec3& v) {
	GLCall(glUniform3f(getUniformLocation(uniformName), v.x, v.y, v.z));
}
void Shader::setUniform4f(const char* uniformName, const glm::vec4& v) {
	GLCall(glUniform4f(getUniformLocation(uniformName), v.x, v.y, v.z, v.w));
}
void Shader::setUniformMat2f(const char* uniformName, const glm::mat2& mat) {
	GLCall(glUniformMatrix2fv(getUniformLocation(uniformName), 1, GL_FALSE, glm::value_ptr(mat)));
}
void Shader::setUniformMat3f(const char* uniformName, const glm::mat3& mat) {
	GLCall(glUniformMatrix3fv(getUniformLocation(uniformName), 1, GL_FALSE, glm::value_ptr(mat)));
}
void Shader::setUniformMat4f(const char* uniformName, const glm::mat4& mat) {
	GLCall(glUniformMatrix4fv(getUniformLocation(uniformName), 1, GL_FALSE, glm::value_ptr(mat)));
}