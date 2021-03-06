#shader vertex

#version 330 core
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

out vec3 Normal;
out vec3 Position;

uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ModelMatrix;

void main() {
	gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * vec4(aPosition, 1.0);

	Position = aPosition;
	Normal = mat3(transpose(inverse(u_ModelMatrix))) * aNormal;
}

#shader fragment

#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform float u_Eta; //Ratio of IORs (e.g. 1.f/1.33f for air -> water)
uniform samplerCube u_SkyboxTex;
uniform vec3 u_CamPos;

void main() {
	vec3 camOffset = normalize(Position - u_CamPos);
	float fresnel = dot(camOffset, Normal);
	vec3 refractVector = refract(camOffset, normalize(Normal), u_Eta);

	vec4 refractColor = texture(u_SkyboxTex, refractVector);
	FragColor = mix(vec4(1, 0, 0, 1), refractColor, 1);
}
