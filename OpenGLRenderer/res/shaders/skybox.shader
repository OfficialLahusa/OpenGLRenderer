#shader vertex
#version 330 core
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

out vec3 TexCoords;
out vec3 NormalTexCoords;

uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;

void main() {
	TexCoords = aPosition;
	NormalTexCoords = normalize(aPosition);
	gl_Position = u_ProjectionMatrix * u_ViewMatrix * vec4(aPosition, 1.0);
}

#shader fragment
#version 330 core
out vec4 FragColor;

in vec3 TexCoords;
in vec3 NormalTexCoords;

uniform samplerCube u_SkyboxTex;

void main() {
	FragColor = texture(u_SkyboxTex, TexCoords);	
}