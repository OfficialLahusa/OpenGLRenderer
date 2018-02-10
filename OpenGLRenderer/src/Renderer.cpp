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

void ResizeWindowCallBack(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void Renderer::Draw(const VertexArray & va, const IndexBuffer & ib, const Shader & shader) const {
	shader.Bind();
	va.Bind();
	ib.Bind();
	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::DrawObj(ObjFile& obj, Material& mat) {
	obj.m_va.Bind();
	obj.m_ib.Bind();
	mat.Bind();
	GLCall(glDrawElements(GL_TRIANGLES, obj.m_ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Clear() {
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

glm::ivec2 Renderer::getWindowSize(GLFWwindow* window) {
	glm::ivec2 result;
	glfwGetWindowSize(window, &result.x, &result.y);
	return result;
}
