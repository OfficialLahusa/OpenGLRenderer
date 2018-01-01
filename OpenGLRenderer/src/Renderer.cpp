#include "Renderer.h"
#include <iostream>

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, unsigned int line)
{
	while (GLenum error = glGetError()) {
		//Output glError in hex form for direct search in glew.h
		std::cout << "[OpenGLError] (0x" << std::hex << error << std::dec << "): " << function << " " << file << ": Line " << line << std::endl;
		return false;
	}
	return true;
}