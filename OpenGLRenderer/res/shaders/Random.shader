#shader vertex
#version 330 core

layout(location = 0) in vec4 position;

void main() {

	gl_Position = position;

}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;
uniform vec4 u_Facts;

void main() {
	color = vec4(fract(sin(dot(gl_FragCoord.xy + u_Facts[0], vec2(12.9898, 78.233))) * 43758.5453),
		fract(sin(dot(gl_FragCoord.xy - u_Facts[0], vec2(12.9898, 78.233))) * 43758.5453),
		fract(sin(dot(gl_FragCoord.xy + 0.5 * u_Facts[0], vec2(12.9898, 78.233))) * 43758.5453),
		1.f
	);
}
