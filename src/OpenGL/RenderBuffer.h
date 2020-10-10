#pragma once

#include "FrameBuffer.h"

class RenderBuffer : public FrameBuffer {
public:
	RenderBuffer() = default;
	~RenderBuffer();

private:
	void createAttachments(int width, int height) override;
	void destroyAttachments() override;
	virtual void attachAttachments() override;

private:
	unsigned int m_colorRenderBufferId;
	unsigned int m_depthRenderBufferId;
};