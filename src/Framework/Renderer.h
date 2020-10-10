#pragma once

#include "OpenGL/QuadVAO.h"
#include "OpenGL/RenderBuffer.h"
#include "OpenGL/TextureFrameBuffer.h"
#include "OpenGL/ShaderPipeline.h"

struct AlphaTrailSettingsValues;

class Renderer {
public:
	Renderer();
	~Renderer() = default;
	// non-copyable
	Renderer(const Renderer& other) = delete;
	Renderer& operator=(const Renderer& other) = delete;

	void onRenderAreaResized(const glm::vec3& clearColor);

	void clearRenderBuffer(const glm::vec3& clearColor);
	void drawFullScreen();
	void drawFullScreenWithUVs();

	inline RenderBuffer& renderBuffer() { return m_renderBuffer; }

private:
	RenderBuffer m_renderBuffer;
	QuadVAO m_fullScreenVAO;
	QuadVAO m_fullScreenVAOWithUVs;
};