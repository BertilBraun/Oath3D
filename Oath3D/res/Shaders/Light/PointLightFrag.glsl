#version 330

#include "Light/LightH.glsl"

uniform sampler2D PositionTexture;
uniform sampler2D DiffuseTexture;
uniform sampler2D NormalTexture;
uniform sampler2D SpecularTexture;

uniform PointLight pointLight;

out vec4 OutColor;

void main()
{
    vec2 TexCoord 	= CalcTexCoord();
    vec4 WorldPos 	= texture(PositionTexture, TexCoord);
    vec4 Color 		= texture(DiffuseTexture, TexCoord);
    vec4 Normal 	= normalize(texture(NormalTexture, TexCoord));
	vec4 spec 		= texture(SpecularTexture, TexCoord);
	
	SpecularIntensity = spec.x + spec.z;
	SpecularPower = spec.y;
	
    OutColor = Color * CalcPointLight(pointLight, WorldPos.xyz, Normal.xyz);
}
