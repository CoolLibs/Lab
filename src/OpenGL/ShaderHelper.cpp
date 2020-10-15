#include "ShaderHelper.h"

#include "Utility/File.h"

std::string ShaderHelper::parseFile(const char* filepath) {
	std::string res;
	MyFile::ToString(filepath, &res);
	return res;
}

unsigned int ShaderHelper::compileShader(unsigned int type, const char* source) {
	GLCall(unsigned int id = glCreateShader(type));
	GLCall(glShaderSource(id, 1, &source, nullptr));
	GLCall(glCompileShader(id));
#ifndef NDEBUG
	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE) {
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)alloca(length * sizeof(char));
		GLCall(glGetShaderInfoLog(id, length, &length, message));
		Log::ErrorWithoutBreakpoint("Failed to compile {} {}", (type == GL_FRAGMENT_SHADER ? "fragment" : "vertex"), "shader");
		Log::Error(message);
		glDeleteShader(id);
		return 0;
	}
#endif
	return id;
}
