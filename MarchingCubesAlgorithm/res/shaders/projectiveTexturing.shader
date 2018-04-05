#shader vertex

#version 330 core
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
out vec4 pass_Position;

uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ModelMatrix;

void main() {
	pass_Position = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * vec4(aPosition, 1.0);
	gl_Position = pass_Position;
	TexCoord = aTexCoord;
}

#shader fragment

#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec4 pass_Position;

uniform sampler2D u_Tex;

void main() {
	FragColor = texture(u_Tex, pass_Position.xy);
}
