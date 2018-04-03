#pragma once
#include "Material.h"
#include "ObjLoader.h"
#include "TransformableMatrix.h"

class GameObject {
public:
	Texture texture;
	PhongMaterial* material;
	Mesh mesh;
	TransformableMatrix transform;

	GameObject(const std::string & mesh_path, const std::string & texture_path, PointLight& light, Camera& cam);
	~GameObject();

	void Bind() const;
	void Unbind() const;
	void Update();
};