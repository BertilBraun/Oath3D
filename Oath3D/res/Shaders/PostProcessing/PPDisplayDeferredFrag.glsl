#version 330

uniform sampler2D PositionTexture;
uniform sampler2D AlbedoTexture;
uniform sampler2D NormalTexture;
uniform sampler2D SpecularTexture;

uniform vec2 ScreenSize;

in vec2 FragTexture;
out vec4 outColor;

void main()
{
	if (gl_FragCoord.x < ScreenSize.x / 2) {
		if (gl_FragCoord.y < ScreenSize.y / 2)
			outColor = texture(PositionTexture, FragTexture * 2);
		else
			outColor = texture(AlbedoTexture, FragTexture * 2);
	}
	else {
		if (gl_FragCoord.y < ScreenSize.y / 2)
			outColor = texture(NormalTexture, FragTexture * 2);
		else
			outColor = texture(SpecularTexture, FragTexture * 2);
	}
}
