#pragma once

class RenderBufferFB;

class Exporter {
public:
	Exporter();
	~Exporter() = default;

	void exportFrame(RenderBufferFB& renderBuffer, std::function<void()> render);

private:
	std::string folderPath;
	std::string fileName;
};