#version 330

layout (location = 0) in vec4 VertPosition;
layout (location = 1) in vec2 VertTexture;

out vec2 FragTexture;

void main() {

	gl_Position = VertPosition;
	FragTexture = VertTexture;
}
