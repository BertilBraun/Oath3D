#version 330

#include "Asset/AssetFragH.glsl"

layout (location = 0) out vec3 WorldPosOut;
layout (location = 1) out vec3 DiffuseOut;
layout (location = 2) out vec3 NormalOut;
layout (location = 3) out vec3 SpecularOut;

uniform sampler2D DiffuseTexture;

uniform sampler2D HeightTexture; //UNUSED
uniform sampler2D NormalTexture;

uniform sampler2D LightTexture; //UNUSED

uniform sampler2D SpecularTexture;
uniform sampler2D SpecularHightlight;

uniform float SpecularExponent;

in vec2 FragTexture;
in vec3 FragNormal;
in vec3 FragTangent;
in vec4 FragColor;
in vec4 FragPos;

in mat3 TBN;

vec3 CalcNormal()
{
    vec3 BumpMapNormal = 2 * texture(NormalTexture, FragTexture).xyz - 1;
	
    return normalize(TBN * BumpMapNormal);
}

void main() {

	//vec2 UVCoord = ParallaxMapping(FragTexture, FragPos.xyz);
	vec2 UVCoord = FragTexture;
	
	vec4 DiffuseColor = texture(DiffuseTexture, UVCoord) + FragColor;
	if (DiffuseColor.a < 0.5)
		discard;
	
	DiffuseOut  = DiffuseColor.rgb;
	WorldPosOut = FragPos.xyz;
	NormalOut 	= CalcNormal();
	SpecularOut = vec3(texture(SpecularTexture, UVCoord).x, SpecularExponent, texture(SpecularHightlight, UVCoord).x);

	//TODO
    NormalOut = normalize(FragNormal);
    //NormalOut = FragTangent;
    //NormalOut = vec3(UVCoord, 0);
}