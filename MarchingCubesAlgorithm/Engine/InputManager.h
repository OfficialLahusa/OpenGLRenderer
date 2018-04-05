#pragma once
#include <GLFW/glfw3.h>
#include <glm\glm.hpp>


class InputManager {

private:

	static glm::vec2 m_lastOffset;
	GLFWwindow* m_window;

	//Callbacks for GLFW Input
	static void defaultMouseCallback(GLFWwindow* window, double xpos, double ypos) {
		int width, height; //Store the Window's position and size
		glfwGetWindowSize(window, &width, &height);
		//glfwGetWindowPos(window, &wposx, &wposy);
		double scrnX = width/2, scrnY = height/2;
		if (xpos <= 3 && xpos >= -3)
			xpos = 0;
		if (ypos <= 3 && ypos >= -3)
			ypos = 0;
		m_lastOffset = glm::vec2(xpos - width/2, -(ypos - height/2));
		glfwSetCursorPos(window, scrnX, scrnY);
	}

public:

	InputManager();
	InputManager(GLFWwindow* window);
	~InputManager();

	bool KeyDown(int glfwKey);
	bool KeyPressed(int glfwKey);
	bool KeyUp(int glfwKey);

	glm::vec2 getMouseOffset();

	void setCursorVisible(bool visible);

	void setWindow(GLFWwindow* window);
};