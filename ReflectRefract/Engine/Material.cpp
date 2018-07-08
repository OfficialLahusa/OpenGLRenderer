#include "Material.h"

PhongMaterial::PhongMaterial(PointLight& light, Camera& cam, Texture& tex)
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

PhongMaterial::~PhongMaterial() {

}

void PhongMaterial::Update() {
	Bind();
	m_shader.setUniform1i("u_Tex", 0);
	m_shader.setUniform3f("u_LightPos", m_light.getPosition());
	m_shader.setUniform3f("u_SpecularColor", m_light.getColor());
	m_shader.setUniformMat4f("u_ViewMatrix", m_cam.GetViewMatrix(), false);
	m_shader.setUniformMat4f("u_ProjectionMatrix", m_cam.GetProjectionMatrix(), false);
	m_shader.setUniformMat4f("u_ModelMatrix", m_model, false);
	m_shader.setUniform3f("u_CamPos", m_cam.Position);
}

void PhongMaterial::loadLight(PointLight light) {
	m_shader.Bind();
	m_shader.setUniform3f("u_LightPos", light.getPosition());
	m_shader.setUniform3f("u_SpecularColor", light.getColor());
	m_light = light;
}

void PhongMaterial::loadCamera(Camera& cam) {
	m_shader.Bind();
	m_shader.setUniformMat4f("u_ViewMatrix", cam.GetViewMatrix(), false);
	m_shader.setUniformMat4f("u_ProjectionMatrix", cam.GetProjectionMatrix(), false);
	m_cam = cam;
}

void PhongMaterial::loadModel(glm::mat4 model) {
	m_shader.Bind();
	m_shader.setUniformMat4f("u_ModelMatrix", model, false);
	m_model = model;
}

void PhongMaterial::setTexture(Texture& tex) {
	m_tex = &tex;
}

void PhongMaterial::Bind() {
	m_tex->Bind(0);
	m_shader.Bind();
}

void PhongMaterial::Unbind() {
	m_shader.Unbind();
	m_tex->Unbind();
}

ReflectMaterial::ReflectMaterial(PointLight& light, Camera& cam, Texture& tex, Cubemap* cm)
	: shader("res/shaders/reflect.shader")
	, light(light)
	, cam(cam)
	, tex(&tex)
	, cubemap(cm)
{

}

ReflectMaterial::~ReflectMaterial() {

}

void ReflectMaterial::Update() {
	Bind();
	//shader.setUniform1i("u_Tex", 0);
	shader.setUniform1i("u_SkyboxTex", 0);
	//shader.setUniform3f("u_LightPos", light.getPosition());
	//shader.setUniform3f("u_SpecularColor", light.getColor());
	shader.setUniformMat4f("u_ViewMatrix", cam.GetViewMatrix(), false);
	shader.setUniformMat4f("u_ProjectionMatrix", cam.GetProjectionMatrix(), false);
	shader.setUniformMat4f("u_ModelMatrix", model, false);
	shader.setUniform3f("u_CamPos", cam.Position);
}

void ReflectMaterial::Bind() {
	tex->Bind(0);
	cubemap->Bind(0);
	shader.Bind();
}

void ReflectMaterial::Unbind() {
	shader.Unbind();
	cubemap->Unbind();
	tex->Unbind();
}

RefractMaterial::RefractMaterial(PointLight& light, Camera& cam, Texture& tex, Cubemap* cm)
	: shader("res/shaders/refract.shader")
	, light(light)
	, cam(cam)
	, tex(&tex)
	, cubemap(cm)
{

}

RefractMaterial::~RefractMaterial() {

}

void RefractMaterial::Update() {
	Bind();
	//shader.setUniform1i("u_Tex", 0);
	shader.setUniform1i("u_SkyboxTex", 0);
	shader.setUniform1f("u_IOR", IOR);
	//shader.setUniform3f("u_LightPos", light.getPosition());
	//shader.setUniform3f("u_SpecularColor", light.getColor());
	shader.setUniformMat4f("u_ViewMatrix", cam.GetViewMatrix(), false);
	shader.setUniformMat4f("u_ProjectionMatrix", cam.GetProjectionMatrix(), false);
	shader.setUniformMat4f("u_ModelMatrix", model, false);
	shader.setUniform3f("u_CamPos", cam.Position);
}

void RefractMaterial::Bind() {
	tex->Bind(0);
	cubemap->Bind(0);
	shader.Bind();
}

void RefractMaterial::Unbind() {
	shader.Unbind();
	cubemap->Unbind();
	tex->Unbind();
}

SphereDissolveMat::SphereDissolveMat(glm::vec3 light, Camera& cam, Texture& tex)
	: m_shader("res/shaders/dissolve.shader")
	, m_light(light)
	, m_cam(cam)
	, m_tex(&tex)
{
	m_shader.Bind();
	m_edgeColor = glm::vec3(0 / 255, 82 / 255, 255 / 255);
}

SphereDissolveMat::~SphereDissolveMat() {

}

void SphereDissolveMat::Update() {
	Bind();
	m_shader.setUniform1i("u_Tex", 0);
	m_shader.setUniform1i("u_NoiseTex", 1);
	
	m_shader.setUniform1f("u_EffectIntensity", m_intensity);
	m_shader.setUniform1f("u_EdgeWidth", m_edgeWidth);
	m_shader.setUniform3f("u_EdgeColor", m_edgeColor);

	m_shader.setUniform3f("u_LightDir", m_light);
	m_shader.setUniformMat4f("u_ViewMatrix", m_cam.GetViewMatrix(), false);
	m_shader.setUniformMat4f("u_ProjectionMatrix", m_cam.GetProjectionMatrix(), false);
	m_shader.setUniformMat4f("u_ModelMatrix", m_model, false);
	m_shader.setUniform3f("u_CamPos", m_cam.Position);
}

void SphereDissolveMat::loadLight(glm::vec3 dir) {
	m_shader.Bind();
	m_shader.setUniform3f("u_LightDir", dir);
	m_light = dir;
}

void SphereDissolveMat::loadCamera(Camera& cam) {
	m_shader.Bind();
	m_shader.setUniformMat4f("u_ViewMatrix", cam.GetViewMatrix(), false);
	m_shader.setUniformMat4f("u_ProjectionMatrix", cam.GetProjectionMatrix(), false);
	m_cam = cam;
}

void SphereDissolveMat::loadModel(glm::mat4 model) {
	m_shader.Bind();
	m_shader.setUniformMat4f("u_ModelMatrix", model, false);
	m_model = model;
}

void SphereDissolveMat::setTexture(Texture& tex) {
	m_tex = &tex;
}

void SphereDissolveMat::setNoiseTexture(Texture& tex) {
	m_noiseTex = &tex;
}

void SphereDissolveMat::Bind() {
	m_tex->Bind(0);
	m_noiseTex->Bind(1);
	m_shader.Bind();
}

void SphereDissolveMat::Unbind() {
	m_shader.Unbind();
	m_tex->Unbind();
}

ProjectiveTexturingMaterial::ProjectiveTexturingMaterial(Camera & cam, Texture & tex)
	: m_shader("res/shaders/projectiveTexturing.shader")
	, m_cam(cam)
	, m_tex(&tex)
{
	m_shader.Bind();
}

ProjectiveTexturingMaterial::~ProjectiveTexturingMaterial() {

}

void ProjectiveTexturingMaterial::Update() {
	Bind();
	m_shader.setUniform1i("u_Tex", 0);
	m_shader.setUniformMat4f("u_ViewMatrix", m_cam.GetViewMatrix(), false);
	m_shader.setUniformMat4f("u_ProjectionMatrix", m_cam.GetProjectionMatrix(), false);
	m_shader.setUniformMat4f("u_ModelMatrix", m_model, false);
	m_shader.setUniform3f("u_CamPos", m_cam.Position);
	Unbind();
}

void ProjectiveTexturingMaterial::loadCamera(Camera& cam) {
	m_shader.Bind();
	m_shader.setUniformMat4f("u_ViewMatrix", cam.GetViewMatrix(), false);
	m_shader.setUniformMat4f("u_ProjectionMatrix", cam.GetProjectionMatrix(), false);
	m_cam = cam;
}

void ProjectiveTexturingMaterial::loadModel(glm::mat4 model) {
	m_shader.Bind();
	m_shader.setUniformMat4f("u_ModelMatrix", model, false);
	m_model = model;
}

void ProjectiveTexturingMaterial::setTexture(Texture & tex) {
	m_tex = &tex;
}

void ProjectiveTexturingMaterial::Bind() {
	m_tex->Bind(0);
	m_shader.Bind();
}

void ProjectiveTexturingMaterial::Unbind() {
	m_shader.Unbind();
	m_tex->Unbind();
}

LightCubeMaterial::LightCubeMaterial(Camera& cam)
	: shader("res/shaders/solidColor3D.shader")
	, cam(cam)
{

}

LightCubeMaterial::~LightCubeMaterial() {

}

void LightCubeMaterial::Update() {
	Bind();
	shader.setUniformMat4f("u_ViewMatrix", cam.GetViewMatrix(), false);
	shader.setUniformMat4f("u_ProjectionMatrix", cam.GetProjectionMatrix(), false);
	shader.setUniformMat4f("u_ModelMatrix", model, false);
	shader.setUniform4f("u_Color", color);
}

void LightCubeMaterial::Bind() {
	shader.Bind();
}

void LightCubeMaterial::Unbind() {
	shader.Unbind();
}
