#pragma once
#include "Cubemap.h"
#include "Shader.h"
#include "ObjLoader.h"

class Skybox {
public:
	Mesh projectionCube;
	Cubemap& cubemap;
	Shader shader;
	Skybox(Cubemap& tex)
		: cubemap(tex)
		, shader("res/shaders/skybox.shader")
		, projectionCube("res/models/skybox.obj")
	{ };
};