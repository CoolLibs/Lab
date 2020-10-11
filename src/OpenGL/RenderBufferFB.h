#pragma once

#include "FrameBuffer.h"

class RenderBufferFB : public FrameBuffer {
public:
	RenderBufferFB() = default;
	~RenderBufferFB();

private:
	void createAttachments(int width, int height) override;
	void destroyAttachments() override;
	void attachAttachments() override;

private:
	GLuint m_colorRenderBufferId = -1;
};