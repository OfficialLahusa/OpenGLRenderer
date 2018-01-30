#include "Shader.h"
#include "Renderer.h"
#include <sstream>
#include <fstream>
#include <iostream>

int Shader::getUniformLocation(const std::string & name) {
	GLCall(unsigned int location = glGetUniformLocation(m_RendererId, name.c_str()));
	if (location == -1) {
		std::cout << "Warning: Uniform " << name << " does not exist.\n";
	}
	
	return location;
}

ShaderProgramSource Shader::ParseShader(const std::string & filepath) {

	std::ifstream stream(filepath);

	enum class ShaderType {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	} type = ShaderType::NONE;

	std::string line;

	std::stringstream ss[2];

	while (getline(stream, line)) {

		if (line.find("#shader") != std::string::npos) {

			if (line.find("vertex") != std::string::npos) {
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos) {
				type = ShaderType::FRAGMENT;
			}

		}
		else {
			ss[(int)type] << line << '\n';
		}

	}

	return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {

	GLCall(unsigned int id = glCreateShader(type));
	const char* src = source.c_str();
	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));

	//Errorcheck
	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE) {
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)alloca(length * sizeof(char));
		GLCall(glGetShaderInfoLog(id, length, &length, message));
		std::cout << "Error: failed to compile "
			<< (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment") << " Shader:\n"
			<< message;

		GLCall(glDeleteShader(id));
		return 0;
	}

	return id;

}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {

	GLCall(unsigned int program = glCreateProgram());
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	return program;
}

Shader::Shader(const std::string & filepath)
	: m_FilePath(filepath)
	, m_RendererId(0)
{
	ShaderProgramSource source = ParseShader(filepath);
	m_RendererId = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader() {
	GLCall(glDeleteProgram(m_RendererId));
}

void Shader::Bind() const {
	GLCall(glUseProgram(m_RendererId));
}

void Shader::Unbind() const {
	GLCall(glUseProgram(0));
}

void Shader::setUniform4f(const std::string & name, glm::vec4 v) {
	GLCall(glUniform4f(getUniformLocation(name), v[0], v[1], v[2], v[3]));
}

void Shader::setUniformMat4f(const std::string & name, glm::mat4 v, bool transpose) {
	GLCall(glUniformMatrix4fv(getUniformLocation(name), 1, (transpose ? GL_TRUE : GL_FALSE), &v[0][0]));
}

void Shader::setUniform1i(const std::string & name, int v) {
	GLCall(glUniform1i(getUniformLocation(name), v));
}
