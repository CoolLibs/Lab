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
	void attachAttachments() override;

private:
	GLuint m_colorTextureId = -1;
};