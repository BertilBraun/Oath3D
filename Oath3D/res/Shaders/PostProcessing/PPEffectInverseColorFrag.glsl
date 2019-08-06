#version 330

uniform sampler2D FinalTexture;
uniform bool InverseColors;


in vec2 FragTexture;
out vec4 outColor;

void main()
{
	if (InverseColors == true)
		outColor = 1 - texture(FinalTexture, FragTexture);
	else
		outColor = texture(FinalTexture, FragTexture);
}
