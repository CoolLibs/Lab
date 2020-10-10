#pragma once

#include "FrameBuffer.h"

class TextureFrameBuffer : public FrameBuffer {
public:
	TextureFrameBuffer() = default;
	~TextureFrameBuffer();

	void attachTextureToSlot(unsigned int slot);

private:
	void createAttachments(int width, int height) override;
	void destroyAttachments() override;
	virtual void attachAttachments() override;

private:
	unsigned int m_colorTextureId;
};