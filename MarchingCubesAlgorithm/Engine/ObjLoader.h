#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"

typedef std::pair<std::vector<float>, std::vector<unsigned int>> fMeshData;

class Mesh {
private:

public:
	Mesh(fMeshData);
	Mesh(const std::string& filepath);
	Mesh();
	~Mesh();

	VertexArray m_va;
	VertexBuffer m_vb;
	VertexBufferLayout m_vbl;
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
	void loadObj(fMeshData& outData, const std::string& filepath);

};