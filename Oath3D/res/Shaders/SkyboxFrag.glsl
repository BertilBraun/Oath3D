#version 330

layout (location = 0) out vec3 WorldPosOut;
layout (location = 1) out vec3 DiffuseOut;
layout (location = 2) out vec3 NormalOut;
layout (location = 3) out vec3 SpecularOut;

in vec3 FragTexture;

uniform samplerCube textureSampler;

void main() {
	
	WorldPosOut = FragTexture;
	DiffuseOut = texture(textureSampler, normalize(FragTexture)).rgb;
	NormalOut = vec3(0, 0, 0);
	SpecularOut = vec3(0);
}
  