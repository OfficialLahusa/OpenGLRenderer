#include "GameObject.h"

GameObject::GameObject(const std::string & mesh_path, const std::string & texture_path, PointLight& light, Camera& cam)
	: mesh(mesh_path)
	, texture(texture_path)
	, material(new PhongMaterial(light, cam, texture))
{
	material->loadModel(transform.getMatrix());
}

GameObject::~GameObject() {
}

void GameObject::Bind() const {
	mesh.m_va.Bind();
	mesh.m_ib.Bind();
	material->Bind();
}

void GameObject::Unbind() const {
	mesh.m_va.Unbind();
	mesh.m_ib.Unbind();
	material->Unbind();
}

void GameObject::Update() {
	material->loadModel(transform.getMatrix());
	material->Update();
}
