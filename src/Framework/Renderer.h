#pragma once

#include "OpenGL/QuadVAO.h"
#include "OpenGL/RenderBufferFB.h"

class Renderer {
public:
	Renderer();
	~Renderer() = default;
	Renderer(const Renderer& other) = delete;			 // non-copyable
	Renderer& operator=(const Renderer& other) = delete; // non-copyable

	void onRenderAreaResized(const glm::vec3& clearColor);

	void clearRenderBuffer(const glm::vec3& clearColor);
	void drawFullScreen();
	void drawFullScreenWithUVs();

	inline RenderBufferFB& renderBuffer() { return m_renderBuffer; }

private:
	RenderBufferFB m_renderBuffer;
	QuadVAO m_fullScreenVAO;
	QuadVAO m_fullScreenVAOWithUVs;
};