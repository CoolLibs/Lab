#include "String.h"

std::string MyString::ToLower(const std::string& str) {
	std::string res = "";
	std::for_each(str.begin(), str.end(), [&res](char c) {
		res += std::tolower(c);
	});
	return res;
}

std::string MyString::RemoveFolderHierarchy(const std::string& myString) {
	auto pos = myString.find_last_of("/\\") + 1;
	return myString.substr(pos, myString.size() - pos);
}

std::string MyString::RemoveFileExtension(const std::string& myString) {
	auto pos = myString.find_last_of(".");
	return myString.substr(0, pos);
}

std::string MyString::FileName(const std::string& myString) {
	return RemoveFolderHierarchy(RemoveFileExtension(myString));
}

std::string MyString::FileExtension(const std::string& myString) {
	auto pos = myString.find_last_of(".");
	return myString.substr(pos, myString.size());
}

bool MyString::StartsWith(const char* toFind, const std::string& str) {
	return str.rfind(toFind, 0) == 0;
}

void MyString::ReplaceAll(std::string& str, const std::string& from, const std::string& to) {
	if (from.empty())
		return;
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
	}
}