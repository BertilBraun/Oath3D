#version 330

layout (location = 0) in vec4 VertPosition;
layout (location = 4) in vec2 VertTexture;

uniform mat4 ViewProj;
uniform mat4 ModelMatrix;

out vec2 FragTexture;

void main() {

	gl_Position		= ViewProj * ModelMatrix * VertPosition;	
	FragTexture		= VertTexture;
}
