#version 330

layout (location = 0) out vec3 WorldPosOut;
layout (location = 1) out vec3 DiffuseOut;
layout (location = 2) out vec3 NormalOut;
layout (location = 3) out vec3 SpecularOut;


in vec3 fs_color;
in vec2 fs_txCoords;
out vec4 color;

uniform sampler2D DiffuseTexture;

void main() {

	WorldPosOut = vec3(0);
	DiffuseOut = texture(DiffuseTexture, fs_txCoords).rgb * fs_color;
	DiffuseOut = vec3(1);
	NormalOut = vec3(0, 1, 0);
	SpecularOut = vec3(0);
}