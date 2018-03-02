#include "Material.h"

PhongMaterial::PhongMaterial()
	: m_shader("res/shaders/phong.shader")
{

}

PhongMaterial::~PhongMaterial() {

}

void PhongMaterial::Update() {

}

void PhongMaterial::loadLight(PointLight light) {
	m_shader.setUniform3f("u_LightPos", light.getPosition());
	m_shader.setUniform3f("u_LightColor", light.getColor());
}

void PhongMaterial::loadCamera(Camera cam) {
	m_shader.setUniformMat4f("u_ViewMatrix", cam.GetViewMatrix(), false);
	m_shader.setUniformMat4f("u_ProjectionMatrix", cam.GetProjectionMatrix(), false);
}

void PhongMaterial::loadModel(glm::mat4 model) {
	m_shader.setUniformMat4f("u_ModelMatrix", model, false);
}

void PhongMaterial::loadTexture(const std::string& filepath) {
	m_tex.loadFromFile(filepath);
}

void PhongMaterial::Bind() {
	m_shader.Bind();
}

void PhongMaterial::Unbind() {
	m_shader.Unbind();
}



OldPhongMaterial::OldPhongMaterial(PointLight& light, Camera& cam, Texture& tex)
	: m_shader("res/shaders/phong_old.shader")
	, m_light(light)
	, m_cam(cam)
	, m_tex(&tex)
{
	m_shader.Bind();
	m_shader.setUniform1f("u_AmbientIntensity", m_ambientIntensity);
	m_shader.setUniform1f("u_DiffuseIntensity", m_diffuseIntensity);
	m_shader.setUniform1f("u_SpecularIntensity", m_specularIntensity);
}

OldPhongMaterial::~OldPhongMaterial() {

}

void OldPhongMaterial::Update() {
	Bind();
	m_shader.setUniform1i("u_Tex", 0);
	m_shader.setUniform3f("u_LightPos", m_light.getPosition());
	m_shader.setUniform3f("u_SpecularColor", m_light.getColor());
	m_shader.setUniformMat4f("u_ViewMatrix", m_cam.GetViewMatrix(), false);
	m_shader.setUniformMat4f("u_ProjectionMatrix", m_cam.GetProjectionMatrix(), false);
	m_shader.setUniformMat4f("u_ModelMatrix", m_model, false);
	m_shader.setUniform3f("u_CamPos", m_cam.Position);
	Unbind();
}

void OldPhongMaterial::loadLight(PointLight light) {
	m_shader.Bind();
	m_shader.setUniform3f("u_LightPos", light.getPosition());
	m_shader.setUniform3f("u_SpecularColor", light.getColor());
	m_light = light;
}

void OldPhongMaterial::loadCamera(Camera cam) {
	m_shader.Bind();
	m_shader.setUniformMat4f("u_ViewMatrix", cam.GetViewMatrix(), false);
	m_shader.setUniformMat4f("u_ProjectionMatrix", cam.GetProjectionMatrix(), false);
	m_cam = cam;
}

void OldPhongMaterial::loadModel(glm::mat4 model) {
	m_shader.Bind();
	m_shader.setUniformMat4f("u_ModelMatrix", model, false);
	m_model = model;
}

void OldPhongMaterial::setTexture(Texture& tex) {
	m_tex = &tex;
}

void OldPhongMaterial::Bind() {
	m_tex->Bind(0);
	m_shader.Bind();
}

void OldPhongMaterial::Unbind() {
	m_shader.Unbind();
	m_tex->Unbind();
}
