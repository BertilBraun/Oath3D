#version 330

uniform sampler2D FinalTexture;

in vec2 FragTexture;

out vec4 OutColor;

void main() { 

	OutColor = texture(FinalTexture, FragTexture);
}