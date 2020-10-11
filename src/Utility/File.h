#pragma once

#include <filesystem>

namespace MyFile {
	bool Exists(const std::string& filepath);
	std::string GetFullPath(const std::string& filepath);
	void ToString(const std::string& filepath, std::string* dst);
	bool CreateFolderIfDoesntExist(const std::string& folderPath); // Returns true iff the folder has been successfully created, or already existed

	const std::string RootDir = std::filesystem::current_path().string();
}