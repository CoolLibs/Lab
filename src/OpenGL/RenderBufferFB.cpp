#include "RenderBufferFB.h"

RenderBufferFB::~RenderBufferFB() {
	destroyAttachments();
}

void RenderBufferFB::createAttachments(int width, int height) {
	FrameBuffer::createAttachments(width, height);
	GLCall(glGenRenderbuffers(1, &m_colorRenderBufferId));
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_colorRenderBufferId));
	GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, width, height));
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
}

void RenderBufferFB::destroyAttachments() {
	FrameBuffer::destroyAttachments();
	GLCall(glDeleteRenderbuffers(1, &m_colorRenderBufferId));
}

void RenderBufferFB::attachAttachments() {
	FrameBuffer::attachAttachments();
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId()));
	GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_colorRenderBufferId));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}