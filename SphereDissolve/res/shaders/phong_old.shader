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

uniform float u_AmbientIntensity = 0.3f;
uniform float u_DiffuseIntensity = 0.8f;
uniform float u_SpecularIntensity = 0.5f;
uniform	float u_Attenuation = 0.5f;
uniform vec3 u_SpecularColor = vec3(1.f, 1.f, 1.f);

uniform vec3 u_LightPos;
uniform vec3 u_CamPos;

void main() {

	vec3 normal = normalize(transpose(inverse(mat3(model))) * fragNormal);
	vec3 surfacePos = vec3(model * vec4(fragPos, 1));
	vec3 surfaceToLight = normalize(u_LightPos - surfacePos);
	float diffuseCoefficient = max(0.0, dot(normal, surfaceToLight));

	vec3 incidenceVector = -surfaceToLight; //a unit vector
	vec3 reflectionVector = reflect(incidenceVector, normal); //also a unit vector
	vec3 surfaceToCamera = normalize(u_CamPos - fragPos); //also a unit vector
	float cosAngle = max(0.0, dot(surfaceToCamera, reflectionVector));
	float specularCoefficient = pow(cosAngle, 1);

	if (texture(u_Tex, fragTexCoord).w == 0.f)
		discard;

	vec4 ambient = texture(u_Tex, fragTexCoord) * u_AmbientIntensity;
	vec4 diffuse = texture(u_Tex, fragTexCoord) * diffuseCoefficient * u_DiffuseIntensity;
	vec4 specular = vec4(specularCoefficient * u_SpecularColor * u_SpecularIntensity, 1.f);

	float attenuation = 1.0 / (1.0 + u_Attenuation * pow(length(u_LightPos - surfacePos), 2));

	FragColor = ambient + attenuation * (diffuse + specular);
	//FragColor = texture(u_Tex, fragTexCoord);
	
}
