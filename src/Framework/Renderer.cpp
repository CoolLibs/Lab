#include "Renderer.h"
#include "Framework/Viewports.h"

Renderer::Renderer()
	: m_fullScreenVAO(false), m_fullScreenVAOWithUVs(true)
{}

void Renderer::begin() {
	m_renderBuffer.bind();
}

void Renderer::end() {
	m_renderBuffer.blitTo(Viewports::SwapYConvention(Viewports::AppView().botLeft()), Viewports::SwapYConvention(Viewports::AppView().topRight()));
	m_renderBuffer.unbind();
}

void Renderer::drawFullScreen() {
	m_fullScreenVAO.bind();
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 3));
}

void Renderer::drawFullScreenWithUVs() {
	m_fullScreenVAOWithUVs.bind();
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 3));
}

void Renderer::onRenderAreaResized() {
	m_renderBuffer.setSize(Viewports::Size().size());
}