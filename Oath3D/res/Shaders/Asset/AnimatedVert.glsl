#version 330

#include "Asset/AssetVertH.glsl"

layout (location = 0) in vec4 Position;
layout (location = 1) in vec4 Normal;
layout (location = 2) in vec3 Tangent;
layout (location = 3) in vec4 Color;
layout (location = 4) in vec2 TexCoord;
layout (location = 5) in uvec4 BoneIDs;
layout (location = 6) in vec4 Weights;

out vec2 FragTexture;
out vec3 FragNormal;
out vec3 FragTangent;
out vec4 FragColor;
out vec4 FragPos;

const int MAX_BONES = 100;

uniform mat4 ProjMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;
uniform mat4 bones[MAX_BONES];
uniform mat3 InvModelMatrix;

void main()
{       
    mat4 BoneTransform = bones[BoneIDs[0]] * Weights[0];
    BoneTransform     += bones[BoneIDs[1]] * Weights[1];
    BoneTransform     += bones[BoneIDs[2]] * Weights[2];
    BoneTransform     += bones[BoneIDs[3]] * Weights[3];

	mat4 ModelTransform = ModelMatrix * BoneTransform;
	mat3 InvTransform = InvModelMatrix * mat3(transpose(inverse(BoneTransform)));
	
    gl_Position		= ProjMatrix * ViewMatrix * ModelTransform * Position;
	
    FragTexture		= TexCoord;
	FragNormal		= InvTransform * Normal.xyz;
	//FragTangent		= (ModelTransform * Tangent).xyz;
	FragTangent		= InvTransform * Tangent.xyz;
	FragColor		= Color;
	FragPos			= ModelTransform * Position;
	
	calcTBN(FragNormal, FragTangent);
}