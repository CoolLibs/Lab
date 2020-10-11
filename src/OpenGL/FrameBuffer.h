#pragma once

static constexpr GLuint SCREEN_FRAMEBUFFER_ID = 0;

class FrameBuffer {
public:
	FrameBuffer();
	virtual ~FrameBuffer();

	void setSize(const glm::ivec2& size);

	void bind();
	void unbind();
	void blitTo(const glm::ivec2& botLeft, const glm::ivec2& topRight, GLuint dstFrameBufferID = SCREEN_FRAMEBUFFER_ID);
	void blitTo(FrameBuffer& frameBuffer);

	inline int width()  const { return m_size.x; }
	inline int height() const { return m_size.y; }
	inline const glm::ivec2& size() const { return m_size; }
	inline float aspectRatio() const { return static_cast<float>(m_size.x) / static_cast<float>(m_size.y); }

protected:
	virtual void destroyAttachments();
	virtual void createAttachments(int width, int height);
	virtual void attachAttachments();

	inline GLuint frameBufferId() { return m_frameBufferId; }

private:
	GLuint m_frameBufferId = -1;
	GLuint m_depthRenderBufferId = -1;
	glm::ivec2 m_size = glm::ivec2(0);
	int m_prevViewport[4];
};