#pragma once

#include "OpenGL/RenderBufferFB.h"
#include "OpenGL/QuadVAO.h"

class Renderer {
public:
	Renderer();
	~Renderer() = default;
	Renderer(const Renderer& other) = delete;			 // non-copyable because QuadVAO is not quite copyable at the moment
	Renderer& operator=(const Renderer& other) = delete; // non-copyable because QuadVAO is not quite copyable at the moment

	void begin();
	void end();

	void drawFullScreen();
	void drawFullScreenWithUVs();

	inline RenderBufferFB& renderBuffer() { return m_renderBuffer; }

	void onRenderAreaResized();

private:
	RenderBufferFB m_renderBuffer;
	QuadVAO m_fullScreenVAO;
	QuadVAO m_fullScreenVAOWithUVs;
};