#include "FileWatcher.h"

#include "Utility/File.h"
#include "Framework/Time.h"

static constexpr float delayBetweenChecks = 0.5f; // In seconds

FileWatcher::FileWatcher(const char* path, std::function<void(const char*)> onFileChanged)
	: m_path(path), m_onFileChanged(onFileChanged), m_timeOfLastChange(std::filesystem::last_write_time(m_path))
{
	m_onFileChanged(m_path.string().c_str());
}

void FileWatcher::update() {
	// Wait for delay between checks
	float t = Time::time();
	if (t - m_timeOfLastCheck > delayBetweenChecks) {
		m_timeOfLastCheck = t;
		// Check file exists
		if (MyFile::Exists(m_path.string())) {
			// Check file was updated since last check
			std::filesystem::file_time_type lastChange = std::filesystem::last_write_time(m_path);
			if (lastChange != m_timeOfLastChange) {
				// Apply
				m_timeOfLastChange = lastChange;
				m_onFileChanged(m_path.string().c_str());
			}
		}
	}
}