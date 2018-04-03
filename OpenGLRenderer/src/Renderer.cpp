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

void Renderer::Draw(Mesh& obj, Material& mat) {
	obj.m_va.Bind();
	obj.m_ib.Bind();
	mat.Bind();
	GLCall(glDrawElements(GL_TRIANGLES, obj.m_ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Draw(GameObject & obj) {
	obj.Bind();
	obj.Update();
	GLCall(glDrawElements(GL_TRIANGLES, obj.mesh.m_ib.GetCount(), GL_UNSIGNED_INT, nullptr));
	obj.Unbind();
}

void Renderer::Draw(Skybox & skybox, glm::mat4 projection, glm::mat4 view) {
	GLCall(glDepthMask(GL_FALSE));
	skybox.shader.Bind();
	skybox.shader.setUniformMat4f("u_ProjectionMatrix", projection, false);
	glm::mat4 skyboxView = glm::mat4(glm::mat3(view));
	skybox.shader.setUniformMat4f("u_ViewMatrix", skyboxView, false);
	skybox.projectionCube.m_va.Bind();
	skybox.projectionCube.m_ib.Bind();
	skybox.cubemap.Bind(0);
	skybox.shader.setUniform1i("u_SkyboxTex", 0);
	GLCall(glDrawElements(GL_TRIANGLES, skybox.projectionCube.m_ib.GetCount(), GL_UNSIGNED_INT, nullptr));
	GLCall(glDepthMask(GL_TRUE));
}

void Renderer::Clear() {
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

glm::ivec2 Renderer::getWindowSize(GLFWwindow* window) {
	glm::ivec2 result;
	glfwGetWindowSize(window, &result.x, &result.y);
	return result;
}
