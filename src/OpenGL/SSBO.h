#pragma once

template <typename T>
class SSBO {
public:
	SSBO(unsigned int binding, GLuint hint)
		: m_binding(binding), m_hint(hint)
	{
		GLCall(glGenBuffers(1, &m_id));
		GLCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_binding, m_id));
	}
	~SSBO() {
		GLCall(glDeleteBuffers(1, &m_id));
	}

	void uploadData(size_t nbOfT, T* data) {
		GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_id));
		GLCall(glBufferData(GL_SHADER_STORAGE_BUFFER, nbOfT * sizeof(T), data, m_hint));
	}

private:
	unsigned int m_binding;
	GLuint m_hint;
	unsigned int m_id;
};