#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 velocity;

uniform mat4 ViewMatrix;

out vec3 vs_color;

const vec3 LO_COLOR = vec3(0xb3, 0xd9, 0xff)/255.f, HI_COLOR = vec3(0xff, 0, 0x66)/255.f;

void main() {
	vs_color = mix(LO_COLOR, HI_COLOR, smoothstep(0.f, 700.f, length(velocity.xyz)));
	gl_Position = ViewMatrix * vec4(position.xyz, 1.f);
}