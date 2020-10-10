#include "Renderer.h"
#include "Framework/Viewports.h"

Renderer::Renderer()
	: m_fullScreenVAO(false), m_fullScreenVAOWithUVs(true)
{}

void Renderer::onRenderAreaResized(const glm::vec3& clearColor) {
	const glm::ivec2& size = Viewports::Size().size();
	m_renderBuffer    .setSize(size.x, size.y);
	clearRenderBuffer(clearColor);
}

void Renderer::clearRenderBuffer(const glm::vec3& clearColor) {
	m_renderBuffer.bind();
	glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	m_renderBuffer.unbind();
}

void Renderer::drawFullScreen() {
	m_fullScreenVAO.bind();
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));
}

void Renderer::drawFullScreenWithUVs() {
	m_fullScreenVAOWithUVs.bind();
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));
}