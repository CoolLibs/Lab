#pragma once

#include "FrameBuffer.h"

class TextureFB : public FrameBuffer {
public:
	TextureFB() = default;
	~TextureFB();

	void attachTextureToSlot(unsigned int slot);

private:
	void createAttachments(int width, int height) override;
	void destroyAttachments() override;
	virtual void attachAttachments() override;

private:
	unsigned int m_colorTextureId;
};