#shader vertex

#version 330 core
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 u_VP;
uniform mat4 u_Model;

void main() {
	gl_Position = u_VP * u_Model * vec4(aPosition, 1.0);
	TexCoord = aTexCoord;
}

#shader fragment

#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D u_Tex;

void main() {
	FragColor = texture(u_Tex, TexCoord);
}
