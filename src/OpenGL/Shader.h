#pragma once

class Shader {
public:
	Shader(const char* vertexShaderFilepath, const char* fragmentShaderFilepath);
	Shader() = default;
	~Shader();
	void compile(const char* vertexShaderFilepath, const char* fragmentShaderFilepath);

	void bind();

	void setUniform1i   (const char* uniformName, int v);
	void setUniform1f   (const char* uniformName, float v);
	void setUniform2f   (const char* uniformName, const glm::vec2& v);
	void setUniform3f   (const char* uniformName, const glm::vec3& v);
	void setUniform4f   (const char* uniformName, const glm::vec4& v);
	void setUniformMat2f(const char* uniformName, const glm::mat2& mat);
	void setUniformMat3f(const char* uniformName, const glm::mat3& mat);
	void setUniformMat4f(const char* uniformName, const glm::mat4& mat);

private:
	std::unordered_map<const char*, GLint> m_uniformLocationCache;
	GLint getUniformLocation(const char* uniformName);

private:
	GLuint m_shaderId = 0;
};