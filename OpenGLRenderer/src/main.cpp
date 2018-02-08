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
#include "Light.h"

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
		ObjLoader objLoader;
		fMeshData objectData, potData, waterData;

		objLoader.loadObj(objectData, "res/models/var1_triangulated.obj");
		objLoader.loadObj(potData, "res/models/pot.obj");
		objLoader.loadObj(waterData, "res/Scenes/Landscapes/test_1/water.obj");

		ObjFile object(objectData);
		ObjFile	pot(potData);
		ObjFile	landscape("res/Scenes/Landscapes/test_1/landscape.obj");
		ObjFile	water(waterData);

		Shader texturedShader("res/shaders/textured.shader");
		Shader oldPhong("res/shaders/phong_old.shader");	//some things derived from: https://www.tomdalling.com/blog/modern-opengl/07-more-lighting-ambient-specular-attenuation-gamma/
		Shader phong("res/shaders/phong.shader");

		Texture grid("res/textures/cGrid.png");
		Texture white("res/textures/white.png");
		Texture dhlPaket("res/textures/variation_2.png");
		Texture landscapeTex("res/Scenes/Landscapes/test_1/landscape_painted.png");
		Texture waterTex("res/Scenes/Landscapes/test_1/waterTex.png");
		

		//Uniform data preparation
		glm::mat4 waterModel, landscapeModel, boxModel, view, projection;
		int wwidth, wheight;

		boxModel = glm::translate(glm::mat4(), glm::vec3(0.f, 0.f, 0.f));
		boxModel = glm::scale(boxModel, glm::vec3(0.2f, 0.2f, 0.2f));

		waterModel = glm::translate(glm::mat4(), glm::vec3(0.f, -2.f, 0.f));
		waterModel = glm::scale(waterModel, glm::vec3(0.1f, 0.1f, 0.1f));

		landscapeModel = glm::translate(glm::mat4(), glm::vec3(0.f, -2.f, 0.f));
		landscapeModel = glm::scale(landscapeModel, glm::vec3(0.1f, 0.1f, 0.1f));


		Camera cam(glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 1.f, 0.f));

		Renderer renderer;

		Clock deltaClock;
		float deltaTime = 0.f;

		glm::vec3 lightPos = {
			0.f, -1.f, 0.f
		};
		float add = 0.3f;

		oldPhong.Bind();
		oldPhong.setUniform1f("u_AmbientIntensity", 0.2f);
		oldPhong.setUniform1f("u_DiffuseIntensity", 0.8f);
		oldPhong.setUniform1f("u_SpecularIntensity", 0.5f);
		oldPhong.setUniform3f("u_SpecularColor", { 1.f, 1.f, 1.f });

		// Window Loop
		while (!glfwWindowShouldClose(window)) {

			glClearColor(0.f, 0.f, 0.1f, 1.f);
			if (lightPos.x >= 3.f) {
				add = -1.f;
			}
			else if (lightPos.x <= -3.f) {
				add = 1.f;
			}
			lightPos.x += add * deltaTime;
			lightPos.z += add * deltaTime;
			oldPhong.setUniform3f("u_LightPos", lightPos);
			oldPhong.setUniform3f("u_CamPos", cam.Position);

			deltaTime = deltaClock.reset();
			/* Render here */
			renderer.Clear();

			view = cam.GetViewMatrix();
			cam.MouseRotate(inputManager.getMouseOffset().x * 0.12f, inputManager.getMouseOffset().y * 0.12f);
			
			glfwGetWindowSize(window, &wwidth, &wheight);
			projection = glm::perspective(glm::radians(80.f), (float)wwidth / (float)wheight, 0.1f, 1000.f);

			boxModel = glm::translate(glm::mat4(), lightPos);
			boxModel = glm::scale(boxModel, glm::vec3(0.2f, 0.2f, 0.2f));
			dhlPaket.Bind(0);
			phong.Bind();
			phong.setUniform1i("u_Tex", 0); //dhlPaket is in unit 0
			phong.setUniformMat4f("u_ViewMatrix", view, false);
			phong.setUniformMat4f("u_ProjectionMatrix", projection, false);
			phong.setUniformMat4f("u_ModelMatrix", boxModel, false);
			renderer.Draw(object.m_va, object.m_ib, phong);
			dhlPaket.Unbind();
		
			waterTex.Bind(0);
			oldPhong.Bind();
			oldPhong.setUniform1i("u_Tex", 0); //waterTex is in unit 0
			oldPhong.setUniformMat4f("u_ViewMatrix", view, false);
			oldPhong.setUniformMat4f("u_ProjectionMatrix", projection, false);
			oldPhong.setUniformMat4f("u_ModelMatrix", waterModel, false);
			renderer.Draw(water.m_va, water.m_ib, oldPhong);
			waterTex.Unbind();

			
			landscapeTex.Bind(0);
			oldPhong.setUniform1i("u_Tex", 0); //landscapeTex is in unit 0
			oldPhong.setUniformMat4f("u_ViewMatrix", view, false);
			oldPhong.setUniformMat4f("u_ProjectionMatrix", projection, false);
			oldPhong.setUniformMat4f("u_ModelMatrix", landscapeModel, false);
			renderer.Draw(landscape.m_va, landscape.m_ib, oldPhong);
			landscapeTex.Unbind();

			/*
			 *Continue here (at 1:48 min)
			 *https://www.youtube.com/watch?v=bcxX0R8nnDs&index=11&list=PLRIWtICgwaX0u7Rf9zkZhLoLuZVfUksDP
			 /



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