#shader vertex

#version 330 core
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

out vec2 fragTexCoord;
out vec3 fragNormal;
out vec3 fragPos;
out mat4 model;


uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ModelMatrix;

void main() {
	gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * vec4(aPosition, 1.0);
	fragTexCoord = aTexCoord;
	fragPos = aPosition;
	fragNormal = aNormal;
	model = u_ModelMatrix;
}

#shader fragment

#version 330 core
out vec4 FragColor;

in vec2 fragTexCoord;
in vec3 fragNormal;
in vec3 fragPos;
in mat4 model;

uniform sampler2D u_Tex;
uniform sampler2D u_NoiseTex;

uniform float u_EffectStrength = 1.f;
uniform float u_EdgeWidth = 0.01;
uniform vec3 u_EdgeColor = vec3(0 / 255, 82 / 255, 255 / 255);

uniform vec3 u_LightDir;
uniform vec3 u_CamPos;

void main() {

	vec3 normal = normalize(transpose(inverse(mat3(model))) * fragNormal);
	vec3 surfacePos = vec3(model * vec4(fragPos, 1));



	if ( texture(u_NoiseTex, fragTexCoord).r <= u_EffectStrength )
		discard;

	vec4 color = max( dot(u_LightDir, normal), 0.3f) * texture(u_Tex, fragTexCoord);

	if (texture(u_NoiseTex, fragTexCoord).r <= u_EffectStrength + u_EdgeWidth)
		color = vec4(u_EdgeColor, 1.f);

	FragColor = color;
	
}
