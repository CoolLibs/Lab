#pragma once

#include <filesystem>

class FileWatcher {
public:
	FileWatcher(const char* path, std::function<void(const char*)> onFileChanged);
	~FileWatcher() = default;

	void update();

private:
	std::filesystem::path m_path;
	std::function<void(const char*)> m_onFileChanged;
	float m_timeOfLastCheck = 0.0f;
	std::filesystem::file_time_type m_timeOfLastChange;
};