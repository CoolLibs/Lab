#pragma once

namespace ShaderHelper {
	std::string parseFile(const char* filepath);
	unsigned int compileShader(unsigned int type, const char* source);
}