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

		Renderer renderer;

		/*ImGui::CreateContext();
		ImGui_ImplGlfwGL3_Init(window, true);
		ImGui::StyleColorsDark();*/

		bool uiCursorMode = true;
		unsigned int uiModeChangeCooldown = 0;
		//inputManager.changeMouseCallback(uiCursorMode);

		ObjLoader objLoader;

		Mesh sphere("res/models/sphere_smooth.obj");
		Mesh lightCube("res/models/skybox.obj");

		Shader texturedShader("res/shaders/textured.shader");
		Shader oldPhong("res/shaders/phong_old.shader");	//some things derived from: https://www.tomdalling.com/blog/modern-opengl/07-more-lighting-ambient-specular-attenuation-gamma/
		Shader reflect("res/shaders/reflect.shader");

		Texture grid("res/textures/cGrid.png");
		Texture brick("res/textures/brick.jpg");
		Texture noise("res/textures/noise.png");
		Texture white("res/textures/white.png");

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
		TransformableMatrix sphereModel(glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f), glm::vec3(0.f, 0.f, 0.f));

		

		//glm::vec3 camCenter = { 0,0,0 };
		//ThirdPersonCamera cam(camCenter, {0,1,0}, 0, 0, 3);
		Camera cam({ 5,5,5 }, { 0,1,0 });
		cam.fov = 80.f;
		cam.aspectRatio = (float)renderer.getWindowSize(window).x / (float)renderer.getWindowSize(window).y;
		cam.near = 0.1f;
		cam.far = 1000.f;

		Clock deltaClock;
		Clock runtimeClock;
		Clock titleUpdateClock;
		float deltaTime = 0.f;

		PointLight light({ 0.f, 0.f, 0.f }, { 1.f, 1.f, 1.f });
		float lightMove = 1.f;

		PhongMaterial phongMat(light, cam, grid);
		ReflectMaterial reflectMat(light, cam, grid, &skyboxTex);
		SphereDissolveMat sphereMat(glm::vec3(1, 1, 1), cam, grid);
		LightCubeMaterial lightCubeMat(cam);

		// Window Loop
		while (!glfwWindowShouldClose(window)) {
			deltaTime = deltaClock.reset();

			if (uiModeChangeCooldown > 0) --uiModeChangeCooldown;

			if(titleUpdateClock.getElapsedTime() >= .75f) {
				std::string title = "Reflections Tech Demo, FPS: "
					+ std::to_string((int)(1 / deltaTime))
					+ "  Cam(" + std::to_string((int)cam.Position.x)
					+ "|" + std::to_string((int)cam.Position.y)
					+ "|" + std::to_string((int)cam.Position.z) + ")";
				glfwSetWindowTitle(window, title.c_str());
				titleUpdateClock.reset();
			}

			glClearColor(0.f, 0.f, 0.1f, 1.f);
			renderer.Clear();
			/*ImGui_ImplGlfwGL3_NewFrame();
			
			ImGui::SetNextWindowSize(ImVec2(500, 100));
			ImGui::Begin("Instruction Window");
			ImGui::Text("Press [TAB] to switch between Camera- and UI-Mode.");
			ImGui::Text("\n\n(C) Lasse Huber-Saffer, 2018");
			ImGui::End();*/

			light.setPosition({ 2.f * sin(runtimeClock.getElapsedTime()), 0, 2.f * cos(runtimeClock.getElapsedTime()) });
			oldPhong.Bind();
			oldPhong.setUniform3f("u_LightPos", light.getPosition());
			oldPhong.setUniform3f("u_CamPos", cam.Position);

			boxModel.setPosition(light.getPosition());
			lightCubeMat.Bind();
			lightCubeMat.color = glm::vec4(light.getColor(), 1.f);
			lightCubeMat.model = boxModel.getMatrix();
			lightCubeMat.Update();
			renderer.Draw(lightCube, lightCubeMat);


			/*sphereMat.setNoiseTexture(noise);
			sphereMat.setTexture(brick);
			sphereMat.setEdgeWidth(0.015f);
			sphereMat.setEdgeColor(glm::vec3(0.1f, 0.5f, 1.f));
			sphereMat.Bind();
			sphereMat.setDissolveIntensity( (sin(runtimeClock.getElapsedTime() * 0.5f) + 1) / 2 );
			sphereMat.loadModel(sphereModel.getMatrix());
			sphereMat.loadCamera(cam);
			sphereMat.loadLight(glm::vec3(1, 1, 1));
			sphereMat.Update();
			renderer.Draw(sphere, sphereMat);*/

			reflectMat.tex = &brick;
			reflectMat.Bind();
			reflectMat.model = sphereModel.getMatrix();
			reflectMat.cam = cam;
			reflectMat.light = light;
			reflectMat.Update();
			renderer.Draw(sphere, reflectMat);

			view = cam.GetViewMatrix();
				
			cam.MouseRotate(inputManager.getMouseOffset().x * 0.12f, inputManager.getMouseOffset().y * 0.12f);
			projection = cam.GetProjectionMatrix();

			boxModel.setPosition(light.getPosition());

			renderer.DrawLine({ 0.f, 0.f, 0.f }, { 1.f, 0.f, 0.f }, { 1.f, 0.f, 0.f }, cam, 3);
			renderer.DrawLine({ 0.f, 0.f, 0.f }, { 0.f, 1.f, 0.f }, { 0.f, 1.f, 0.f }, cam, 3);
			renderer.DrawLine({ 0.f, 0.f, 0.f }, { 0.f, 0.f, 1.f }, { 0.f, 0.f, 1.f }, cam, 3);

			renderer.Draw(skybox, projection, view);

			/*ImGui::Render();
			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());*/

			glfwSwapBuffers(window);
			glfwPollEvents();


			//Keyboard Events
			if (inputManager.KeyDown(GLFW_KEY_ESCAPE)) {
				goto deleteWindow;
			}
			if (inputManager.KeyDown(GLFW_KEY_W)) {
				cam.Translate(cam.Front * 5.f * deltaTime);
			}
			if (inputManager.KeyDown(GLFW_KEY_S)) {
				cam.Translate(cam.Front * -5.f * deltaTime);
			}
			if (inputManager.KeyDown(GLFW_KEY_A)) {
				cam.Translate(cam.Right * -5.f * deltaTime);
			}
			if (inputManager.KeyDown(GLFW_KEY_D)) {
				cam.Translate(cam.Right * 5.f * deltaTime);
			}
			if (inputManager.KeyDown(GLFW_KEY_SPACE)) {
				cam.lookAt(glm::vec3(553, 109, 11));
			}
			/*if (inputManager.KeyDown(GLFW_KEY_TAB) && uiModeChangeCooldown == 0) {
				uiCursorMode = !uiCursorMode;
				inputManager.changeMouseCallback(uiCursorMode);
				uiModeChangeCooldown += 30;
			}*/
			
		}

	}

deleteWindow:

	/*ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();*/
	glfwTerminate();
	return 0;
}