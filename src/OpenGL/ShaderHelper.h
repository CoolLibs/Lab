#pragma once

namespace ShaderHelper {
	std::string parseFile(const std::string& filepath, const std::vector<std::pair<std::string, std::string>>& modifyFromTo = {});
	unsigned int compileShader(unsigned int type, const std::string& source);
}