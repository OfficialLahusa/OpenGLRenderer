#pragma once
#include "Shader.h"
#include "Texture.h"
#include "Light.h"
#include "Camera.h"

#include <vector>
#include <glm/glm.hpp>

class Material {

private:
	

public:
	virtual void Update() = 0;

	virtual void Bind() = 0;
	virtual void Unbind() = 0;
};

class PhongMaterial : public Material {
private:
	Shader m_shader;
	PointLight& m_light;
	Camera& m_cam;
	glm::mat4 m_model;

	float m_ambientIntensity = 0.2f;
	float m_diffuseIntensity = 0.8f;
	float m_specularIntensity = 0.5f;

public:
	PhongMaterial(PointLight& light, Camera& cam, Texture& tex);
	~PhongMaterial();

	void Update() override;

	Texture* m_tex;

	void loadLight(PointLight light);
	void loadCamera(Camera cam);
	void loadModel(glm::mat4 model);
	void setTexture(Texture& tex);

	void setAmbIntensity(float v) {
		m_ambientIntensity = v;
		m_shader.setUniform1f("u_AmbientIntensity", v);
	}
	void setDiffIntensity(float v) {
		m_diffuseIntensity = v;
		m_shader.setUniform1f("u_DiffuseIntensity", v);
	}
	void setSpecIntensity(float v) {
		m_specularIntensity = v;
		m_shader.setUniform1f("u_SpecularIntensity", v);
	}
	float getAmbIntensity() {
		return m_ambientIntensity;
	}
	float getDiffIntensity() {
		return m_diffuseIntensity;
	}
	float getSpecIntensity() {
		return m_specularIntensity;
	}

	void Bind() override;
	void Unbind() override;
};