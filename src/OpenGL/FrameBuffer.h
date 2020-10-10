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
	inline GLuint frameBufferId() { return m_frameBufferId; }

private:
	virtual void destroyAttachments() = 0;
	virtual void createAttachments(int width, int height) = 0;
	virtual void attachAttachments() = 0;

private:
	GLuint m_frameBufferId;
	glm::ivec2 m_size;
	int m_prevViewportSettings[4];
};