#version 330

#include "Light/LightH.glsl"

uniform sampler2D PositionTexture;
uniform sampler2D DiffuseTexture;
uniform sampler2D NormalTexture;
uniform sampler2D SpecularTexture;

uniform DirectionalLight directionalLight;

in vec2 FragTexture;

out vec4 OutColor;

void main()
{
    vec4 WorldPos 	= texture(PositionTexture, FragTexture);
    vec4 Color 		= texture(DiffuseTexture, FragTexture);
    vec4 Normal 	= normalize(texture(NormalTexture, FragTexture));
	vec4 spec 		= texture(SpecularTexture, FragTexture);
	
	SpecularIntensity = spec.x + spec.z;
	SpecularPower = spec.y;
	
    OutColor = Color * CalcDirectionalLight(directionalLight, WorldPos.xyz, Normal.xyz);
}
