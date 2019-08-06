#version 330

layout (location = 0) in vec4 VertPosition;
layout (location = 4) in vec2 VertTexture;
layout (location = 5) in mat4 ModelMatrix;

uniform mat4 ViewProj;

out vec2 FragTexture;

void main() {

	gl_Position		= ViewProj * ModelMatrix * VertPosition;	
	FragTexture		= VertTexture;
}
