#include "Exporter.h"

#include <Cool/App/RenderState.h>
#include <Cool/File/File.h>
#include <Cool/ExportImage/AsPNG.h>
#include <Cool/OpenGL/RenderBufferFB.h>

namespace Cool {

Exporter::Exporter()
	: folderPath(File::RootDir + "/exports"), fileName("test.png")
{}

void Exporter::exportFrame(RenderBufferFB& renderBuffer, std::function<void()> render) {
	RenderState::setIsExporting(true);
	int w = RenderState::Size().width();
	int h = RenderState::Size().height();
	// Get pixel data
	render();
	renderBuffer.bind();
	unsigned char* data = new unsigned char[4 * w * h];
	glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, data);
	renderBuffer.unbind();
	// Write png
	File::CreateFoldersIfDoesntExist(folderPath.c_str());
	ExportImage::AsPNG((folderPath + "/" + fileName).c_str(), w, h, data);
	delete[] data;
	//
	RenderState::setIsExporting(false);
}

} // namespace Cool