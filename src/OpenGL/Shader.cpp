#include "Shader.h"

#include <fstream>
#include <iostream>

Shader::Shader(ShaderType type, const std::string& filepathOrSourceCode, bool bIsFilepath)
	: m_filepathOrSourceCode(filepathOrSourceCode), m_bIsFilepath(bIsFilepath)
{
	switch (type)
	{
	case ShaderType::Vertex:
		m_type = GL_VERTEX_SHADER;
		break;
	case ShaderType::Fragment:
		m_type = GL_FRAGMENT_SHADER;
		break;
	case ShaderType::Compute:
		m_type = GL_COMPUTE_SHADER;
		break;
	default:
		spdlog::warn("[Shader::Shader] Unknown shader type : {}", (int)type);
		break;
	}
}

Shader::Shader(Shader&& other) noexcept
	: m_type(other.m_type),
	  m_filepathOrSourceCode(other.m_filepathOrSourceCode),
	  m_bIsFilepath(other.m_bIsFilepath),
	  m_shaderID(other.m_shaderID)
{
	other.m_shaderID = 0;
}

void Shader::operator=(Shader&& o) {
	m_type = o.m_type;
	m_filepathOrSourceCode = std::move(o.m_filepathOrSourceCode);
	m_bIsFilepath = o.m_bIsFilepath;
	m_shaderID = o.m_shaderID;
	//
	o.m_shaderID = 0;
}

Shader::~Shader() {
	glDeleteShader(m_shaderID);
}

void Shader::compile() {
	m_shaderID = glCreateShader(m_type);
	std::string srcStr = m_bIsFilepath ? parseFile(m_filepathOrSourceCode) : m_filepathOrSourceCode;
	const char* src = srcStr.c_str();
	glShaderSource(m_shaderID, 1, &src, nullptr);
	glCompileShader(m_shaderID);
	//Debug
	int result;
	glGetShaderiv(m_shaderID, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(m_shaderID, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(m_shaderID, length, &length, message);
		spdlog::warn("[Shader::compile] failed");
		spdlog::warn(message);
		glDeleteShader(m_shaderID);
	}
}

std::string Shader::parseFile(const std::string& filepath) {
	std::ifstream stream(filepath);
	//spdlog::info("Opening '{}'", filepath);
	if (!stream.is_open()) {
		spdlog::warn("[Shader::parseFile] Failed to open file : '{}'", filepath);
		return "";
	}

	std::string str = "";
	std::string tempLine = "";
	while (getline(stream, tempLine)) {
		str += tempLine + '\n';
	}
	stream.close();
	return str;
}