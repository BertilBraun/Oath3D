#version 330

uniform sampler2D DiffuseTexture;

in vec2 FragTexture;

void main() {

	if (texture(DiffuseTexture, FragTexture).a < 0.2)
		discard;
}