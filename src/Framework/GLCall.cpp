#include "GLCall.h"

void glexp::clear() {
	while (glGetError() != GL_NO_ERROR);
}

bool glexp::doesFunctionWork(const char *functionName, const char *filename, int line) {
	GLenum error;
	while ((error = glGetError()) != GL_NO_ERROR) {
		Log::Error("[OpenGL Error] {}: {} {} {}", glErrorString(error), functionName, filename, line);
		return false;
	}
	return true;
}

char const* glexp::glErrorString(GLenum const err) {
	switch (err) {
	case GL_NO_ERROR:
		return "GL_NO_ERROR";

	case GL_INVALID_ENUM:
		return "GL_INVALID_ENUM";

	case GL_INVALID_VALUE:
		return "GL_INVALID_VALUE";

	case GL_INVALID_OPERATION:
		return "GL_INVALID_OPERATION";

	case GL_OUT_OF_MEMORY:
		return "GL_OUT_OF_MEMORY";

	case GL_INVALID_FRAMEBUFFER_OPERATION:
		return "GL_INVALID_FRAMEBUFFER_OPERATION";

	default:
		assert(!"unknown error");
		return nullptr;
	}
}
