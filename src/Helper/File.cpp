#include "File.h"

#include <sys/stat.h>
#include <fstream>
#include <streambuf>

bool MyFile::Exists(const std::string& filepath) {
	struct stat buffer;
	return (stat(filepath.c_str(), &buffer) == 0);
}

std::string MyFile::GetFullPath(const std::string& filepath) {
	return std::filesystem::absolute(filepath).string();
}

void MyFile::ToString(const std::string& filepath, std::string* dst) {
    // Thanks to https://stackoverflow.com/questions/2602013/read-whole-ascii-file-into-c-stdstring
    std::ifstream stream(filepath);
    if (!stream.is_open()) {
        spdlog::warn("[MyFile::ToString] Failed to open file : '{}'", filepath);
        return;
    }
    stream.seekg(0, std::ios::end);
    dst->reserve(stream.tellg());
    stream.seekg(0, std::ios::beg);
    dst->assign(
        (std::istreambuf_iterator<char>(stream)),
        std::istreambuf_iterator<char>()
    );
    stream.close();
}

bool MyFile::CreateFolderIfDoesntExist(const std::string& folderPath) {
    if (!Exists(folderPath)) {
        try {
            std::filesystem::create_directories(folderPath);
            return true;
        }
        catch (std::exception e) {
            spdlog::warn(e.what());
            return false;
        }
    }
    return true;
}