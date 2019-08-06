#version 330

#include "Asset/AssetVertH.glsl"

layout (location = 0) in vec4 VertPosition;
layout (location = 1) in vec4 VertNormal;
layout (location = 2) in vec4 VertTangent;
layout (location = 3) in vec4 VertColor;
layout (location = 4) in vec2 VertTexture;

uniform mat4 ProjMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;
uniform mat3 InvModelMatrix;

out vec2 FragTexture;
out vec3 FragNormal;
out vec3 FragTangent;
out vec4 FragColor;
out vec4 FragPos;

void main() {

	gl_Position		= ProjMatrix * ViewMatrix * ModelMatrix * VertPosition;	
	FragTexture		= VertTexture;
	FragNormal		= InvModelMatrix * VertNormal.xyz;
	//FragTangent		= (ModelMatrix * VertTangent).xyz;
	FragTangent		= InvModelMatrix * VertTangent.xyz;
	FragColor		= VertColor;
	FragPos			= ModelMatrix * VertPosition;
	
	calcTBN(FragNormal, FragTangent);
}