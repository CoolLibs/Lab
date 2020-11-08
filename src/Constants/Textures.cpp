#include "Textures.h"

#include <stb_image/stb_image.h>

GLuint Textures::m_playID  = -1;
GLuint Textures::m_pauseID = -1;

void Textures::_LoadAll() {
	m_playID  = LoadTexture("icons/play.png");
	m_pauseID = LoadTexture("icons/pause.png");
}

void Textures::_DestroyAll() {
	DestroyTexture(m_playID);
	DestroyTexture(m_pauseID);
}

GLuint Textures::LoadTexture(const std::string& filepath) {
	// Load image
	stbi_set_flip_vertically_on_load(0);
	int width, height;
	unsigned char* data = stbi_load(filepath.c_str(), &width, &height, nullptr, 4);
	if (!data) {
		Log::Error("Couldn't open file '{}'", filepath);
		return -1;
	}
	// Create texture
	GLuint texID;
	GLCall(glGenTextures(1, &texID));
	GLCall(glBindTexture(GL_TEXTURE_2D, texID));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	// Cleanup
	stbi_image_free(data);
	//
	return texID;
}

void Textures::DestroyTexture(unsigned int texID) {
	GLCall(glDeleteTextures(1, &texID));
}