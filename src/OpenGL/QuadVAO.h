#pragma once

class QuadVAO {
public:
	QuadVAO(bool bWithUVs = false); // Full Screen Quad
	~QuadVAO();

	void bind();

private:
	unsigned int m_vaoID;
	unsigned int m_vboID;
};