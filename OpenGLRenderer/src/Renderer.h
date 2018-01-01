#pragma once

#include <GL/glew.h>

#define ASSERT(x) if(!(x)) __debugbreak();

//#x turns x into a string
#ifdef _DEBUG
#define GLCall(x) GLClearError(); x; ASSERT(GLLogCall(#x, __FILE__, __LINE__)); 
#else
#define GLCall(x) x;
#endif

void GLClearError();

bool GLLogCall(const char* function, const char* file, unsigned int line);
