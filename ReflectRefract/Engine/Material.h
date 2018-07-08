#pragma once
#include "Shader.h"
#include "Texture.h"
#include "Light.h"
#include "Camera.h"
#include "Cubemap.h"

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
	void loadCamera(Camera& cam);
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

class ReflectMaterial : public Material {
private:
	

public:
	ReflectMaterial(PointLight& light, Camera& cam, Texture& tex, Cubemap* sb);
	~ReflectMaterial();

	void Update() override;

	Texture* tex;
	Cubemap* cubemap;

	Shader shader;
	PointLight& light;
	Camera& cam;
	glm::mat4 model;

	float ambientIntensity = 0.2f;
	float diffuseIntensity = 0.8f;
	float specularIntensity = 0.5f;
	float reflectivity = 0.5f;


	void Bind() override;
	void Unbind() override;
};

class RefractMaterial : public Material {
private:


public:
	RefractMaterial(PointLight& light, Camera& cam, Texture& tex, Cubemap* sb);
	~RefractMaterial();

	void Update() override;

	Texture* tex;
	Cubemap* cubemap;

	Shader shader;
	PointLight& light;
	Camera& cam;
	glm::mat4 model;

	float ambientIntensity = 0.2f;
	float diffuseIntensity = 0.8f;
	float specularIntensity = 0.5f;
	float reflectivity = 0.5f;

	float IOR = 1.f / 1.52f;


	void Bind() override;
	void Unbind() override;
};

class LightCubeMaterial : public Material {
private:


public:
	LightCubeMaterial(Camera& cam);
	~LightCubeMaterial();

	void Update() override;

	glm::vec4 color;

	Shader shader;
	Camera& cam;
	glm::mat4 model;


	void Bind() override;
	void Unbind() override;
};

class SphereDissolveMat : public Material {
private:
	Shader m_shader;
	glm::vec3 m_light;
	Camera& m_cam;
	glm::mat4 m_model;

	float m_edgeWidth = 0.01f;
	glm::vec3 m_edgeColor;
	float m_intensity = 1.f;

public:
	SphereDissolveMat(glm::vec3 light, Camera& cam, Texture& tex);
	~SphereDissolveMat();

	void Update() override;

	Texture* m_tex;
	Texture* m_noiseTex;

	void loadLight(glm::vec3 dir);
	void loadCamera(Camera& cam);
	void loadModel(glm::mat4 model);
	void setTexture(Texture& tex);
	void setNoiseTexture(Texture& tex);

	void setDissolveIntensity(float v) {
		m_intensity = v;
		m_shader.setUniform1f("u_EffectStrength", v);
	}
	void setEdgeWidth(float v) {
		m_edgeWidth = v;
	}
	void setEdgeColor(glm::vec3 v) {
		m_edgeColor = v;
	}

	float getDissolveIntensity() {
		return m_intensity;
	}
	float getEdgeWidth() {
		return m_edgeWidth;
	}
	glm::vec3 getEdgeColor() {
		return m_edgeColor;
	}

	void Bind() override;
	void Unbind() override;
};

class ProjectiveTexturingMaterial : public Material {
private:
	Shader m_shader;
	Camera& m_cam;
	glm::mat4 m_model;

public:
	ProjectiveTexturingMaterial(Camera& cam, Texture& tex);
	~ProjectiveTexturingMaterial();

	void Update() override;

	Texture* m_tex;

	void loadCamera(Camera& cam);
	void loadModel(glm::mat4 model);
	void setTexture(Texture& tex);

	void Bind() override;
	void Unbind() override;
};