#pragma once

#include "RectSize.h"

class RectSizePos : public RectSize {
public:
	RectSizePos(int width = 0, int height = 0, int topLeftX = 0, int topLeftY = 0)
		: RectSize(width, height), m_topLeft(topLeftX, topLeftY)
	{}

	inline glm::ivec2 topLeft()  const { return m_topLeft; }                               // y-axis points down
	inline glm::ivec2 botRight() const { return m_topLeft + size(); }                      // y-axis points down
	inline glm::ivec2 botLeft()  const { return { m_topLeft.x, m_topLeft.y + size().y }; } // y-axis points down
	inline glm::ivec2 topRight() const { return { m_topLeft.x + size().x, m_topLeft.y }; } // y-axis points down

	inline void setTopLeft(int topLeftX, int topLeftY) { m_topLeft.x = topLeftX; m_topLeft.y = topLeftY; }

private:
	glm::ivec2 m_topLeft; // y-axis points down
};