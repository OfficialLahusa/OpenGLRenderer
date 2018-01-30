#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 color;
out vec3 fragmentColor;

uniform mat4 u_VP;
uniform mat4 u_Model;

void main() {

	gl_Position = u_VP * u_Model * vec4(position.xyz, 1.f);
	fragmentColor = color;

}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;
in vec3 fragmentColor;

void main() {
	color = vec4(fragmentColor, 1.0f);
}
