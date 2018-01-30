#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "GameObject.h"
#include "InputManager.h"
#include "Camera.h"
#include "Texture.h"
#include "ObjLoader.h"
#include "Clock.h"

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(16 * 8 * 10, 9 * 8 * 10, "Schloss Pinneberg Engine", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}


	// Make the window's context current
	glfwMakeContextCurrent(window);

	InputManager inputManager(window);
	inputManager.setCursorVisible(false);

	//glfwSwapInterval(1); now replaced with clock

	glfwSetWindowSizeCallback(window, ResizeWindowCallBack);
	GLCall(glEnable(GL_DEPTH_TEST));

	// glewInit works after Context creation (1 line above)
	GLenum err = glewInit();

	if (GLEW_OK != err) {
		std::cout << "Error: glew not ok" << std::endl;
		return -1;
	}
	else {
		std::cout << "GlewInit successful" << std::endl;
	}

	// Print version
	GLCall(std::cout << glGetString(GL_VERSION) << std::endl);

	{
		float positions[] = {	//x, y, z, r, g, b
			-0.5f, -0.5f, 0.5f,		1.f, 0.f, 0.f,
			0.5f, -0.5f, 0.5f,		0.f, 1.f, 0.f,
			0.5f, 0.5f, 0.f,		0.f, 0.f, 1.f,
			-0.5f, 0.5f, 0.f,		1.f, 1.f, 0.f,
			0.5f, -0.5f, -0.5f,		0.f, 1.f, 0.f,
			-0.5f, -0.5f, -0.5f,	1.f, 0.f, 0.f,
			-0.5f, -0.5f, 0.f,		1.f, 0.f, 0.f, // lines one and two with z = 0.f
			0.5f, -0.5f, 0.f,		0.f, 1.f, 0.f,
		};

		float facePositions[] = { //x, y, z, u, v
			-0.5f, -0.5f, 0.f,		0.f, 0.f,
			0.5f, -0.5f, 0.f,		1.f, 0.f,
			0.5f, 0.5f, 0.f,		1.f, 1.f,
			-0.5f, 0.5f, 0.f,		0.f, 1.f
		};

		GLuint indices[] = {
			0,1,2,
			2,3,0,
			5,4,2,
			2,3,5,
			0,1,4,
			4,5,0
		};

		GLuint faceIndices[] = {
			0, 1, 2,
			2, 3, 0
		};


		
		VertexArray cubeVA;
		VertexBuffer cubeVB(&positions, 8 * 6 * sizeof(float));
		VertexBufferLayout cubeLayout;
		cubeLayout.Push<float>(3);
		cubeLayout.Push<float>(3);
		cubeVA.AddBuffer(cubeVB, cubeLayout);

		VertexArray faceVA;
		VertexBuffer faceVB(&facePositions, 8 * 5 * sizeof(float));
		VertexBufferLayout faceLayout;
		faceLayout.Push<float>(3);
		faceLayout.Push<float>(2);
		faceVA.AddBuffer(faceVB, faceLayout);

		IndexBuffer cubeIB(indices, 18);
		IndexBuffer faceIB(faceIndices, 6);

		ObjLoader objLoader;
		ObjFile object = objLoader.loadObj("res/models/var1_triangulated.obj");

		Shader BasicShader("res/shaders/Basic.shader");
		Shader RandomShader("res/shaders/Random.shader");
		Shader cubeShader("res/shaders/vertexColor.shader");
		Shader faceShader("res/shaders/textured.shader");

		Texture grid;
		grid.loadFromFile("res/textures/cGrid.png");

		//Uniform data preparation
		glm::mat4 cubeModel, faceModel, view, projection, vp;
		int wwidth, wheight;

		faceModel = glm::translate(glm::mat4(), glm::vec3(2.f, 0.f, 0.f));


		Camera cam(glm::vec3(0, 0, 2), glm::vec3(0, 1, 0));

		Renderer renderer;

		Clock deltaClock;
		float deltaTime = 0.f;

		// Window Loop
		while (!glfwWindowShouldClose(window)) {

			deltaTime = deltaClock.reset();
			/* Render here */
			renderer.Clear();

			view = cam.GetViewMatrix();
			cam.MouseRotate(inputManager.getMouseOffset().x * 0.12f, inputManager.getMouseOffset().y * 0.12f);
			
			glfwGetWindowSize(window, &wwidth, &wheight);
			projection = glm::perspective(glm::radians(80.f), (float)wwidth / (float)wheight, 0.1f, 1000.f);
			vp = projection * view;

			cubeShader.Bind();
			cubeShader.setUniformMat4f("u_VP", vp, false);
			cubeShader.setUniformMat4f("u_Model", cubeModel, false);
			renderer.Draw(cubeVA, cubeIB, cubeShader);

			grid.Bind(0);
			faceShader.Bind();
			faceShader.setUniform1i("u_Tex", 0); //grid is in unit 0
			faceShader.setUniformMat4f("u_VP", vp, false);
			faceShader.setUniformMat4f("u_Model", faceModel, false);
			renderer.Draw(faceVA, faceIB, faceShader);
			grid.Unbind();

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();

			if (inputManager.KeyDown(GLFW_KEY_ESCAPE)) {
				goto deleteWindow;
			}

			if (inputManager.KeyDown(GLFW_KEY_W))
				cam.Translate(Camera_Movement::FORWARD, deltaTime);
			if (inputManager.KeyDown(GLFW_KEY_S))
				cam.Translate(Camera_Movement::BACKWARD, deltaTime);
			if (inputManager.KeyDown(GLFW_KEY_A))
				cam.Translate(Camera_Movement::LEFT, deltaTime);
			if (inputManager.KeyDown(GLFW_KEY_D))
				cam.Translate(Camera_Movement::RIGHT, deltaTime);

		}

	}

	deleteWindow:
	glfwTerminate();
	return 0;
}