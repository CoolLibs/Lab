#include "Renderer.h"
#include "RenderState.h"

Renderer::Renderer() {
	GLCall(glGenVertexArrays(1, &m_dummyVaoID));
}
Renderer::~Renderer() {
	GLCall(glDeleteVertexArrays(1, &m_dummyVaoID));
}

void Renderer::begin() {
	m_renderBuffer.bind();
}

void Renderer::end() {
	m_renderBuffer.blitTo(RenderState::SwapYConvention(RenderState::InAppRenderArea().botLeft()), RenderState::SwapYConvention(RenderState::InAppRenderArea().topRight()));
	m_renderBuffer.unbind();
}

void Renderer::dummyDrawCallForFullscreen() {
	GLCall(glBindVertexArray(m_dummyVaoID));
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 3));
}

void Renderer::onRenderAreaResized() {
	m_renderBuffer.setSize(RenderState::Size().size());
}