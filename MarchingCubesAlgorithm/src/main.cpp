#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "../Engine/Renderer.h"
#include "../Engine/VertexBuffer.h"
#include "../Engine/VertexBufferLayout.h"
#include "../Engine/IndexBuffer.h"
#include "../Engine/VertexArray.h"
#include "../Engine/Shader.h"
#include "../Engine/InputManager.h"
#include "../Engine/Camera.h"
#include "../Engine/Texture.h"
#include "../Engine/ObjLoader.h"
#include "../Engine/Clock.h"
#include "../Engine/Light.h"
#include "../Engine/TransformableMatrix.h"
#include "../Engine/Cubemap.h"
#include "../Engine/Skybox.h"

#define FULLSCREEN false

int main(void) {

	GLFWwindow* window = InitializeOpenGL(FULLSCREEN);

	//GL Settings
	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glDepthFunc(GL_LEQUAL));

	{
		InputManager inputManager(window);
		inputManager.setCursorVisible(false);

		ObjLoader objLoader;

		//Mesh testMesh("res/models/pot.obj");	

		Shader texturedShader("res/shaders/textured.shader");
		Shader oldPhong("res/shaders/phong_old.shader");	//some things derived from: https://www.tomdalling.com/blog/modern-opengl/07-more-lighting-ambient-specular-attenuation-gamma/
		Shader reflect("res/shaders/reflect.shader");

		Texture grid("res/textures/cGrid.png");

		Cubemap skyboxTex({ "res/cubemaps/skybox/right.jpg",
			"res/cubemaps/skybox/left.jpg",
			"res/cubemaps/skybox/top.jpg",
			"res/cubemaps/skybox/bottom.jpg",
			"res/cubemaps/skybox/front.jpg",
			"res/cubemaps/skybox/back.jpg",
		});
		Skybox skybox(skyboxTex);

		//Uniform data preparation
		glm::mat4 view, projection;
		int wwidth, wheight;

		TransformableMatrix boxModel(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.f, 0.f, 0.f));
		TransformableMatrix testModel (glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f), glm::vec3(0.f, 0.f, 0.f));

		Renderer renderer;

		Camera cam(glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
		cam.fov = 80.f;
		cam.aspectRatio = (float)renderer.getWindowSize(window).x / (float)renderer.getWindowSize(window).y;
		cam.near = 0.1f;
		cam.far = 1000.f;

		Clock deltaClock;
		Clock runtimeClock;
		float deltaTime = 0.f;

		PointLight light({ 0.f, 0.f, 0.f }, { 1.f, 1.f, 1.f });
		float lightMove = 1.f;

		PhongMaterial phongMat(light, cam, grid);
		phongMat.Bind();

		/*oldPhong.Bind();
		oldPhong.setUniform1f("u_AmbientIntensity", 0.2f);
		oldPhong.setUniform1f("u_DiffuseIntensity", 0.8f);
		oldPhong.setUniform1f("u_SpecularIntensity", 0.5f);
		oldPhong.setUniform3f("u_SpecularColor", { 1.f, 1.f, 1.f });*/

		// Window Loop
		while (!glfwWindowShouldClose(window)) {
			deltaTime = deltaClock.reset();

			std::string title = "Marching Cubes Algorithm Tech Demo, FPS: " + std::to_string((int)(1 / deltaTime));
			glfwSetWindowTitle(window, title.c_str());


			glClearColor(0.f, 0.f, 0.1f, 1.f);
			renderer.Clear();

			/* Move the Light */
			light.setPosition({ 2.f * sin(runtimeClock.getElapsedTime()), -1.f, 2.f * cos(runtimeClock.getElapsedTime()) });
			oldPhong.Bind();
			oldPhong.setUniform3f("u_LightPos", light.getPosition());
			oldPhong.setUniform3f("u_CamPos", cam.Position);

			view = cam.GetViewMatrix();
			cam.MouseRotate(inputManager.getMouseOffset().x * 0.12f, inputManager.getMouseOffset().y * 0.12f);
			projection = cam.GetProjectionMatrix();

			boxModel.setPosition(light.getPosition());

			renderer.Draw(skybox, projection, view);

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