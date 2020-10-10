#include "ShaderHelper.h"

#include "Helper/String.h"
#include "Helper/File.h"

std::string ShaderHelper::parseFile(const std::string& filepath, const std::vector<std::pair<std::string, std::string>>& modifyFromTo) {
	std::string res;
	MyFile::ToString(filepath, &res);
	for (const auto& pair : modifyFromTo)
		MyString::ReplaceAll(res, pair.first, pair.second);
	return res;
}

unsigned int ShaderHelper::compileShader(unsigned int type, const std::string& source) {
	GLCall(unsigned int id = glCreateShader(type));
	const char* src = source.c_str();
	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));
	// Check for errors
	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE) {
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)alloca(length * sizeof(char));
		GLCall(glGetShaderInfoLog(id, length, &length, message));
		Log::Error("Failed to compile {} {}", (type == GL_FRAGMENT_SHADER ? "fragment" : "vertex"), "shader");
		Log::Error(message);
		glDeleteShader(id);
		return 0;
	}
	//
	return id;
}
