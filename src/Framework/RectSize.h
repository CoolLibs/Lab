#pragma once

class RectSize {
public:
	RectSize(int width = 0, int height = 0)
		: m_size(width, height)
	{
		computeAspectRatio();
	}
	~RectSize() = default;

	inline int width() const              { return m_size.x; }
	inline int height() const             { return m_size.y; }
	inline const glm::ivec2& size() const { return m_size; }
	inline float aspectRatio() const      { return m_aspectRatio; }

	inline void setSize(int width, int height) { m_size.x = width; m_size.y = height; computeAspectRatio(); }

private:
	inline void computeAspectRatio() { 
		m_aspectRatio = m_size.y == 0 ? 0.0f : static_cast<float>(m_size.x) / static_cast<float>(m_size.y);
	}

private:
	glm::ivec2 m_size;
	float m_aspectRatio;
};