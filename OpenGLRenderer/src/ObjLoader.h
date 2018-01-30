#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "VertexArray.h"
#include "IndexBuffer.h"

class ObjFile {
private:

public:
	ObjFile() {};
	~ObjFile() {};

	VertexArray m_va;
	IndexBuffer m_ib;

};

class ObjLoader {

private:
	std::vector<glm::vec3> m_positions;
	std::vector<glm::vec3> m_normals;
	std::vector<glm::vec2> m_textureCoordinates;
	std::vector<glm::vec3> m_finalVerts; //Vertices extracted from "f" lines in the obj file;

public:
	void clearMemory();
	ObjFile loadObj(const std::string& filepath);

};