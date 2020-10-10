#include "RenderBufferFB.h"

RenderBufferFB::~RenderBufferFB() {
	destroyAttachments();
}

void RenderBufferFB::createAttachments(int width, int height) {
	// Color attachment
	GLCall(glGenRenderbuffers(1, &m_colorRenderBufferId));
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_colorRenderBufferId));
	GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, width, height));
	// Depth attachment
	GLCall(glGenRenderbuffers(1, &m_depthRenderBufferId));
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_depthRenderBufferId));
	GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height));
	//
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
}

void RenderBufferFB::destroyAttachments() {
	GLCall(glDeleteRenderbuffers(1, &m_colorRenderBufferId));
	GLCall(glDeleteRenderbuffers(1, &m_depthRenderBufferId));
}

void RenderBufferFB::attachAttachments() {
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, getFrameBufferId()));
	GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_colorRenderBufferId));
	GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthRenderBufferId));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}