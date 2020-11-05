#include "Exporter.h"

#include "RenderState.h"
#include "OpenGL/RenderBufferFB.h"
#include "Utility/File.h"
#include <stb_image/stb_image_write.h>

Exporter::Exporter()
	: folderPath(MyFile::RootDir + "/exports"), fileName("test.png")
{}

void Exporter::exportFrame(RenderBufferFB& renderBuffer, std::function<void()> render) {
	int w = RenderState::getExportSize().x;
	int h = RenderState::getExportSize().y;
	RenderState::setIsExporting(true);
	// Get pixel data
	render();
	renderBuffer.bind();
	unsigned char* data = new unsigned char[4 * w * h];
	glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, data);
	renderBuffer.unbind();
	// Write png
	stbi_flip_vertically_on_write(1);
	MyFile::CreateFolderIfDoesntExist(folderPath.c_str());
	stbi_write_png((folderPath + "/" + fileName).c_str(), w, h, 4, data, 0);
	delete[] data;
	//
	RenderState::setIsExporting(false);
}