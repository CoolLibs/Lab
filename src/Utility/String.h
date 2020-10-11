#pragma once

namespace MyString {
	std::string ToLower(const std::string& str);
	std::string RemoveFolderHierarchy(const std::string& myString);
	std::string RemoveFileExtension(const std::string& myString);
	std::string FileName(const std::string& myString);
	std::string FileExtension(const std::string& myString);
	bool StartsWith(const char* toFind, const std::string& str);
	void ReplaceAll(std::string& str, const std::string& from, const std::string& to);
	template<typename T>
	std::string ToString(T val, int minNbChars = -1) { // adds 0s to the left to reach minNbChars (if the latter isn't -1)
		std::string str = std::to_string(val);
		if (minNbChars != -1) {
			int dn = minNbChars - static_cast<int>(str.size());
			if (dn > 0) {
				str = std::string(dn, '0') + str;
			}
		}
		return str;
	}
}