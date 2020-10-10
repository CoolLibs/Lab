#include "Renderer.h"
#include "Framework/Viewports.h"

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
	m_renderBuffer.blitTo(Viewports::SwapYConvention(Viewports::AppView().botLeft()), Viewports::SwapYConvention(Viewports::AppView().topRight()));
	m_renderBuffer.unbind();
}

void Renderer::dummyDrawCallForFullscreen() {
	GLCall(glBindVertexArray(m_dummyVaoID));
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 3));
}

void Renderer::onRenderAreaResized() {
	m_renderBuffer.setSize(Viewports::Size().size());
}