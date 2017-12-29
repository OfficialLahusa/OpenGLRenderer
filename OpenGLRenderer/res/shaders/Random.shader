#shader vertex
#version 330 core

layout(location = 0) in vec4 position;

void main() {

	gl_Position = position;

}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

void main() {
	color = vec4(fract(sin(dot(gl_FragCoord.xy, vec2(12.9898, 78.233))) * 43758.5453),
		fract(sin(dot(gl_FragCoord.xy + 10, vec2(12.9898, 78.233))) * 43758.5453),
		fract(sin(dot(gl_FragCoord.xy + 30, vec2(12.9898, 78.233))) * 43758.5453),
		1.0);
}
