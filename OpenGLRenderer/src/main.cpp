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
#include "TransformableMatrix.h"
#include "Cubemap.h"
#include "Skybox.h"

#define WINDOWED false

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

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
	if(WINDOWED)
		window = glfwCreateWindow(mode->width, mode->height, "Schloss Pinneberg Engine", primaryMonitor, NULL);
	else
		window = glfwCreateWindow(mode->width, mode->height, "Schloss Pinneberg Engine", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	// Make the window's context current
	glfwMakeContextCurrent(window);
	glfwSetWindowSizeCallback(window, ResizeWindowCallBack);


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
	GLCall(glEnable(GL_DEPTH_TEST));

	{
		InputManager inputManager(window);
		inputManager.setCursorVisible(false);

		ObjLoader objLoader;
		fMeshData objectData, potData;

		objLoader.loadObj(objectData, "res/models/var1_triangulated.obj");
		objLoader.loadObj(potData, "res/models/pot.obj");

		Mesh object(objectData);
		Mesh pot(potData);
		Mesh landscape("res/Scenes/Landscapes/test_1/landscape.obj");
		Mesh water("res/Scenes/Landscapes/test_1/water.obj");
		Mesh floor("res/Themes/FancyDungeon/preview/floor.obj");
		Mesh walls("res/Themes/FancyDungeon/preview/walls.obj");
		//Mesh dragon("res/models/dragon_personalUse/con-armadura.obj");
		Mesh fern("res/models/fern_1_gameready.obj");
		Mesh woodenFloor("res/models/construction/wooden_planks.obj");
		Mesh icosphere("res/models/icosphere.obj");

		Shader texturedShader("res/shaders/textured.shader");
		Shader oldPhong("res/shaders/phong_old.shader");	//some things derived from: https://www.tomdalling.com/blog/modern-opengl/07-more-lighting-ambient-specular-attenuation-gamma/
		Shader reflect("res/shaders/reflect.shader");

		Texture grid("res/textures/cGrid.png");
		Texture white("res/textures/white.png");
		Texture dhlPaket("res/textures/variation_2.png");
		Texture landscapeTex("res/Scenes/Landscapes/test_1/landscape_painted.png");
		Texture waterTex("res/Scenes/Landscapes/test_1/waterTex.png");
		Texture floorTex("res/Themes/FancyDungeon/DefaultRooms/FloorTile/1_tex.png");
		Texture wallTex("res/Themes/FancyDungeon/DefaultRooms/WallTile/1_tex.png");
		Texture fernTex("res/textures/fern.png");
		Texture bareWoodenPlanks("res/textures/wood_bare.jpg");

		Cubemap skyboxTex({ "res/cubemaps/skybox/right.jpg",
			"res/cubemaps/skybox/left.jpg",
			"res/cubemaps/skybox/top.jpg",
			"res/cubemaps/skybox/bottom.jpg",
			"res/cubemaps/skybox/front.jpg",
			"res/cubemaps/skybox/back.jpg",
		});
		Skybox skybox(skyboxTex);

		//Uniform data preparation
		glm::mat4 dragonModel, view, projection;
		int wwidth, wheight;

		TransformableMatrix boxModel(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.f, 0.f, 0.f));
		TransformableMatrix waterModel(glm::vec3(0.f, -2.f, 0.f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.f, 0.f, 0.f));
		TransformableMatrix landscapeModel(glm::vec3(0.f, -2.f, 0.f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.f, 0.f, 0.f));
		TransformableMatrix dungeonModel(glm::vec3(0.f, -1.f, 0.f), glm::vec3(1.f, 1.f, 1.f), glm::vec3(0.f, 0.f, 0.f));

		/*dragonModel = glm::rotate(glm::mat4(), 0.08f, glm::vec3(1.f, 0.f, 0.f));
		dragonModel = glm::scale(dragonModel, glm::vec3(0.3f, 0.3f, 0.3f));
		dragonModel = glm::translate(dragonModel, glm::vec3(0.f, -5.7f, 0.f));*/
		

		Renderer renderer;

		Camera cam(glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
		cam.fov = 80.f;
		cam.aspectRatio = (float)renderer.getWindowSize(window).x / (float)renderer.getWindowSize(window).y;
		cam.near = 0.1f;
		cam.far = 1000.f;

		Clock deltaClock;
		Clock runtimeClock;
		float deltaTime = 0.f;

		PointLight light({ 0.f, -1.f, 0.f }, { 1.f, 1.f, 1.f });
		float lightMove = 1.f;

		PhongMaterial phongMat(light, cam, landscapeTex);
		phongMat.Bind();
		phongMat.loadModel(dungeonModel.getMatrix());

		ProjectiveTexturingMaterial doorMat(cam, grid);
		doorMat.loadModel(dungeonModel.getMatrix());

		oldPhong.Bind();
		oldPhong.setUniform1f("u_AmbientIntensity", 0.2f);
		oldPhong.setUniform1f("u_DiffuseIntensity", 0.8f);
		oldPhong.setUniform1f("u_SpecularIntensity", 0.5f);
		oldPhong.setUniform3f("u_SpecularColor", { 1.f, 1.f, 1.f });

		// Window Loop
		while (!glfwWindowShouldClose(window)) {
			deltaTime = deltaClock.reset();
			
			std::string title = "Schloss Pinneberg Engine, FPS: " + std::to_string((int)(1 / deltaTime));
			glfwSetWindowTitle(window, title.c_str());
			

			glClearColor(0.f, 0.f, 0.1f, 1.f);
			renderer.Clear();

			/* Move the Light */
			light.setPosition({2.f * sin(runtimeClock.getElapsedTime()), -1.f, 2.f * cos(runtimeClock.getElapsedTime())});
			oldPhong.setUniform3f("u_LightPos", light.getPosition());
			oldPhong.setUniform3f("u_CamPos", cam.Position);

			view = cam.GetViewMatrix();
			cam.MouseRotate(inputManager.getMouseOffset().x * 0.12f, inputManager.getMouseOffset().y * 0.12f);
			projection = cam.GetProjectionMatrix();

			boxModel.setPosition(light.getPosition());
			
			/* Render here */
			renderer.Draw(skybox, projection, view);

			/*phongMat.m_tex = &wallTex;
			phongMat.loadModel(dragonModel);
			phongMat.Update();
			renderer.Draw(dragon, phongMat);

			phongMat.m_tex = &floorTex;
			phongMat.loadModel(dungeonModel);
			phongMat.Update();
			renderer.Draw(walls, phongMat);

			phongMat.m_tex = &fernTex;
			phongMat.loadModel(landscapeModel);
			phongMat.Update();
			renderer.Draw(fern, phongMat);*/

			/*phongMat.m_tex = &bareWoodenPlanks;
			phongMat.loadModel(dungeonModel.getMatrix());
			phongMat.Update();
			renderer.Draw(woodenFloor, phongMat);*/

			dhlPaket.Bind(0);
			texturedShader.Bind();
			texturedShader.setUniform1i("u_Tex", 0); //dhlPaket is in unit 0
			texturedShader.setUniformMat4f("u_ViewMatrix", view, false);
			texturedShader.setUniformMat4f("u_ProjectionMatrix", projection, false);
			texturedShader.setUniformMat4f("u_ModelMatrix", boxModel.getMatrix(), false);
			renderer.Draw(object.m_va, object.m_ib, texturedShader);
			dhlPaket.Unbind();
		
			waterTex.Bind(0);
			oldPhong.Bind();
			oldPhong.setUniform1i("u_Tex", 0); //waterTex is in unit 0
			oldPhong.setUniformMat4f("u_ViewMatrix", view, false);
			oldPhong.setUniformMat4f("u_ProjectionMatrix", projection, false);
			oldPhong.setUniformMat4f("u_ModelMatrix", waterModel.getMatrix(), false);
			renderer.Draw(water.m_va, water.m_ib, oldPhong);
			waterTex.Unbind();

			
			landscapeTex.Bind(0);
			oldPhong.setUniform1i("u_Tex", 0); //landscapeTex is in unit 0
			oldPhong.setUniformMat4f("u_ViewMatrix", view, false);
			oldPhong.setUniformMat4f("u_ProjectionMatrix", projection, false);
			oldPhong.setUniformMat4f("u_ModelMatrix", landscapeModel.getMatrix(), false);
			renderer.Draw(landscape.m_va, landscape.m_ib, oldPhong);
			landscapeTex.Unbind();

			

			/*floorTex.Bind(0);
			oldPhong.setUniform1i("u_Tex", 0); //landscapeTex is in unit 0
			oldPhong.setUniformMat4f("u_ViewMatrix", view, false);
			oldPhong.setUniformMat4f("u_ProjectionMatrix", projection, false);
			oldPhong.setUniformMat4f("u_ModelMatrix", dungeonModel, false);
			renderer.Draw(floor.m_va, floor.m_ib, oldPhong);
			floorTex.Unbind();

			wallTex.Bind(0);
			oldPhong.setUniform1i("u_Tex", 0); //landscapeTex is in unit 0
			oldPhong.setUniformMat4f("u_ViewMatrix", view, false);
			oldPhong.setUniformMat4f("u_ProjectionMatrix", projection, false);
			oldPhong.setUniformMat4f("u_ModelMatrix", dungeonModel, false);
			renderer.Draw(walls.m_va, walls.m_ib, oldPhong);
			wallTex.Unbind();*/

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
			
			/*
			if (inputManager.KeyDown(GLFW_KEY_KP_ADD))
				
			if (inputManager.KeyDown(GLFW_KEY_KP_SUBTRACT))
			*/
		}

	}

	deleteWindow:
	glfwTerminate();
	return 0;
}