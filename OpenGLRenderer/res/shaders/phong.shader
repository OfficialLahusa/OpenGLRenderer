#shader vertex

#version 330 core
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

out vec2 fragTexCoord;
out vec3 fragNormal;
out vec3 fragPos;
out mat4 model;


uniform mat4 u_VP;
uniform mat4 u_Model;

void main() {
	gl_Position = u_VP * u_Model * vec4(aPosition, 1.0);
	fragTexCoord = aTexCoord;
	fragPos = aPosition;
	fragNormal = aNormal;
	model = u_Model;
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
uniform	float u_Attenuation = 1.f;
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


	vec3 ambient = texture(u_Tex, fragTexCoord).rgb * u_AmbientIntensity;
	vec3 diffuse = texture(u_Tex, fragTexCoord).rgb * diffuseCoefficient * u_DiffuseIntensity;
	vec3 specular = specularCoefficient * u_SpecularColor * u_SpecularIntensity;

	float attenuation = 1.0 / (1.0 + u_Attenuation * pow(length(u_LightPos - surfacePos), 2));

	FragColor = vec4(ambient + attenuation * (diffuse + specular), 1.0f);
	
}
