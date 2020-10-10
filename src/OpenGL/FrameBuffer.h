#pragma once

class FrameBuffer {
public:
	FrameBuffer();
	virtual ~FrameBuffer();

	virtual void setSize(int width, int height);

	void bind();
	void unbind();
	void blitToScreen(const glm::ivec2& botLeft, const glm::ivec2& topRight);
	void blitTo(FrameBuffer& frameBuffer);

	inline int width()  const { return m_width; }
	inline int height() const { return m_height; }
	inline float aspectRatio() const { return static_cast<float>(m_width) / m_height; }

protected:
	inline unsigned int getFrameBufferId() { return m_frameBufferId; }

private:
	virtual void destroyAttachments() = 0;
	virtual void createAttachments(int width, int height) = 0;
	virtual void attachAttachments() = 0;

private:
	unsigned int m_frameBufferId;
	int m_prevViewportSettings[4];
	int m_width;
	int m_height;
};