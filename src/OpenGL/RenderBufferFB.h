#pragma once

#include "FrameBuffer.h"

class RenderBufferFB : public FrameBuffer {
public:
	RenderBufferFB() = default;
	~RenderBufferFB();

private:
	void createAttachments(int width, int height) override;
	void destroyAttachments() override;
	virtual void attachAttachments() override;

private:
	unsigned int m_colorRenderBufferId;
	unsigned int m_depthRenderBufferId;
};