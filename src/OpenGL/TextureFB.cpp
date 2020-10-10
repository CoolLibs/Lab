#include "TextureFB.h"

TextureFB::~TextureFB() {
	destroyAttachments();
}

void TextureFB::createAttachments(int width, int height) {
	FrameBuffer::createAttachments(width, height);
	GLCall(glGenTextures(1, &m_colorTextureId));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_colorTextureId));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void TextureFB::destroyAttachments() {
	FrameBuffer::destroyAttachments();
	GLCall(glDeleteTextures(1, &m_colorTextureId));
}

void TextureFB::attachAttachments() {
	FrameBuffer::attachAttachments();
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId()));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_colorTextureId));
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorTextureId, 0));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void TextureFB::attachTextureToSlot(unsigned int slot) {
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_colorTextureId));
}