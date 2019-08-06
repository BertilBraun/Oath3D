#version 330

layout (location = 0) in vec4 VertPosition;

out vec3 FragTexture;

uniform mat4 ProjMatrix;
uniform mat4 ViewMatrix;

void main() {

	mat4 view = ViewMatrix;
	view[3][0] = 0;
	view[3][1] = 0;
	view[3][2] = 0;

	gl_Position = ProjMatrix * view * VertPosition;
		
	FragTexture	= VertPosition.xyz;
}
