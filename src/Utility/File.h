#pragma once

#include <filesystem>

namespace MyFile {
	bool Exists(const char* filepath);
	std::string GetFullPath(const char* filepath);
	void ToString(const char* filepath, std::string* dst);
	bool CreateFolderIfDoesntExist(const char* folderPath); // Returns true iff the folder has been successfully created, or already existed

	const std::string RootDir = std::filesystem::current_path().string();
}