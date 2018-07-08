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

GLFWwindow* InitializeOpenGL(bool fullscreen) {

	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit()) {
		std::cerr << "[Error] GLFW Init failed.\n";
		return 0;
	}	

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);

	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

	/* Create a windowed mode window and its OpenGL context */
	if (fullscreen)
		window = glfwCreateWindow(mode->width, mode->height, "Schloss Pinneberg Engine", primaryMonitor, NULL);
	else
		window = glfwCreateWindow(mode->width, mode->height, "Schloss Pinneberg Engine", NULL, NULL);
	if (!window) {
		glfwTerminate();
		std::cerr << "[Error] Window creation failed.\n";
		return 0;
	}

	// Make the window's context current
	glfwMakeContextCurrent(window);
	glfwSetWindowSizeCallback(window, ResizeWindowCallBack);


	// glewInit works after Context creation (1 line above)
	GLenum err = glewInit();

	if (GLEW_OK != err) {
		std::cerr << "[Error] Glew Init failed.\n";
		return 0;
	}
	else {
		std::cout << "GlewInit successful" << std::endl;
	}

	// Print version
	GLCall(std::cout << glGetString(GL_VERSION) << std::endl);

	return window;
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

void Renderer::DrawLine(glm::vec3 pos1, glm::vec3 pos2, glm::vec3 color, Camera& cam, float width) {
	glm::mat4 view = cam.GetViewMatrix(), projection = cam.GetProjectionMatrix();
	LineShader.Bind();
	LineShader.setUniform4f("u_Color", glm::vec4(color.r, color.g, color.b, 1.f));
	LineShader.setUniformMat4f("u_ViewMatrix", view, false);
	LineShader.setUniformMat4f("u_ProjectionMatrix", projection, false);
	
	float data[6]{
		pos1.x, pos1.y, pos1.z,
		pos2.x, pos2.y, pos2.z
	};
	unsigned int indices[2]{
		0, 1
	};

	VertexBuffer buf;
	buf.load(&data, 6 * sizeof(float));
	VertexBufferLayout layout;
	layout.Push<float>(3);
	VertexArray va;
	va.AddBuffer(buf, layout);
	IndexBuffer ib(indices, 2 * sizeof(unsigned int));
	va.Bind();
	ib.Bind();
	glLineWidth(width);
	GLCall(glDrawElements(GL_LINES, ib.GetCount() - 1 * sizeof(float), GL_UNSIGNED_INT, nullptr)); //-3 is needed to work somehow
	glDrawArrays(GL_LINES, 0, 2);
	glLineWidth(1);
	va.Unbind();
	ib.Unbind();
}

void Renderer::Clear() {
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

glm::ivec2 Renderer::getWindowSize(GLFWwindow* window) {
	glm::ivec2 result;
	glfwGetWindowSize(window, &result.x, &result.y);
	return result;
}
