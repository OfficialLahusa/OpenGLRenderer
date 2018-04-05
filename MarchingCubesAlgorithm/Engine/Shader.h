#pragma once
#include <string>
#include <glm/glm.hpp>
#include <unordered_map>

struct ShaderProgramSource {
	std::string VertexSource, FragmentSource;
};

class Shader {
private:
	unsigned int m_RendererId;
	std::string m_FilePath;
	std::unordered_map<std::string, unsigned int> m_locations;

	int getUniformLocation(const std::string& name);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	ShaderProgramSource ParseShader(const std::string & filepath);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	//Uniform management;
	void setUniform4f(const std::string& name, glm::vec4 v);
	void setUniform3f(const std::string& name, glm::vec3 v);
	void setUniformMat4f(const std::string & name, glm::mat4 v, bool transpose);
	void setUniform1i(const std::string& name, int v);
	void setUniform1f(const std::string& name, float v);
};