#include "FrameBuffer.h"

FrameBuffer::FrameBuffer()
{
	GLCall(glGenFramebuffers(1, &m_frameBufferId));
}

FrameBuffer::~FrameBuffer() {
	destroyAttachments();
	GLCall(glDeleteFramebuffers(1, &m_frameBufferId));
}

void FrameBuffer::setSize(const glm::ivec2& size) {
	m_size = size;
	destroyAttachments();
	createAttachments(size.x, size.y);
	attachAttachments();
#ifndef NDEBUG
	bind();
	GLCall(auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER));
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		const char* statusStr;
		switch (status) {
		case GL_FRAMEBUFFER_UNDEFINED:
			statusStr = "GL_FRAMEBUFFER_UNDEFINED";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			statusStr = "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT ";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			statusStr = "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT ";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
			statusStr = "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER ";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
			statusStr = "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER ";
			break;
		case GL_FRAMEBUFFER_UNSUPPORTED:
			statusStr = "GL_FRAMEBUFFER_UNSUPPORTED ";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
			statusStr = "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE ";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
			statusStr = "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS ";
			break;
		default:
			statusStr = "UNKNOWN_ERROR";
			break;
		}
		Log::Error("Framebuffer is not complete : {}", statusStr);
	}
	unbind();
#endif
}

void FrameBuffer::bind() {
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferId));
	GLCall(glGetIntegerv(GL_VIEWPORT, m_prevViewport)); // Store viewport settings to restore them when unbinding
	GLCall(glViewport(0, 0, width(), height()));                // Only usefull if we plan on using this frame buffer at a different resolution than the screen's
}

void FrameBuffer::unbind() {
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, SCREEN_FRAMEBUFFER_ID));
	GLCall(glViewport(m_prevViewport[0], m_prevViewport[1], m_prevViewport[2], m_prevViewport[3]));
}

void FrameBuffer::blitTo(const glm::ivec2& botLeft, const glm::ivec2& topRight, GLuint dstFrameBufferID) {
	GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dstFrameBufferID));
	GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, m_frameBufferId));
	GLCall(glBlitFramebuffer(0, 0, width(), height(), botLeft.x, botLeft.y, topRight.x, topRight.y, GL_COLOR_BUFFER_BIT, GL_LINEAR));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void FrameBuffer::blitTo(FrameBuffer& frameBuffer) {
	blitTo({ 0, 0 }, frameBuffer.size(), frameBuffer.frameBufferId());
}

void FrameBuffer::createAttachments(int width, int height) {
	GLCall(glGenRenderbuffers(1, &m_depthRenderBufferId));
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_depthRenderBufferId));
	GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height));
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));
}

void FrameBuffer::destroyAttachments() {
	GLCall(glDeleteRenderbuffers(1, &m_depthRenderBufferId));
}

void FrameBuffer::attachAttachments() {
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId()));
	GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthRenderBufferId));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}