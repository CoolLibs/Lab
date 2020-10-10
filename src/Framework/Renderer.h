#pragma once

#include "OpenGL/RenderBufferFB.h"

class Renderer {
public:
	Renderer();
	~Renderer();
	Renderer(const Renderer& other) = delete;			 // non-copyable because it would destroy the VAO (we would need move operators)
	Renderer& operator=(const Renderer& other) = delete; // non-copyable because it would destroy the VAO (we would need move operators)

	void begin();
	void end();

	void dummyDrawCallForFullscreen(); // We use a smart trick to render fullscreen, as explained here : https://stackoverflow.com/a/59739538

	inline RenderBufferFB& renderBuffer() { return m_renderBuffer; }

	void onRenderAreaResized();

private:
	RenderBufferFB m_renderBuffer;
	GLuint m_dummyVaoID;
};