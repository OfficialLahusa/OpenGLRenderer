#pragma once

#include <GL/glew.h>
#include <GLFW\glfw3.h>

#include <glm/glm.hpp>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "ObjLoader.h"
#include "Material.h"


#define ASSERT(x) if(!(x)) __debugbreak();

//#x turns x into a string
#ifdef _DEBUG
#define GLCall(x) GLClearError(); x; ASSERT(GLLogCall(#x, __FILE__, __LINE__)); 
#else
#define GLCall(x) x;
#endif

void GLClearError();

bool GLLogCall(const char* function, const char* file, unsigned int line);

void ResizeWindowCallBack(GLFWwindow* window, int width, int height);

class Renderer {
private:

public:
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
	void DrawObj(ObjFile& obj, Material& mat);
	void Clear();
	glm::ivec2 getWindowSize(GLFWwindow* window);
};
