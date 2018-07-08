#shader vertex

#version 330 core
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

out mat4 model;
out vec3 reflectVector;
out vec3 color;


uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ModelMatrix;

uniform vec3 u_CamPos;

void main() {
	gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * vec4(aPosition, 1.0);
	model = u_ModelMatrix;

	vec3 camOffset = aPosition - u_CamPos;
	reflectVector = reflect(camOffset, aNormal);

	color = vec3(u_CamPos.x, u_CamPos.x, u_CamPos.x);
}

#shader fragment

#version 330 core
out vec4 FragColor;

in vec3 reflectVector;
in vec3 color;
in mat4 model;

uniform samplerCube u_SkyboxTex;

void main() {

	//FragColor = texture(u_SkyboxTex, reflectVector);
	FragColor = vec4(color, 1.f);

}
