#version 330

layout (location = 0) in vec4 Position;
layout (location = 4) in vec2 TexCoord;
layout (location = 5) in uvec4 BoneIDs;
layout (location = 6) in vec4 Weights;

out vec2 FragTexture;

const int MAX_BONES = 100;

uniform mat4 ViewProj;
uniform mat4 ModelMatrix;
uniform mat4 bones[MAX_BONES];

void main()
{       
    mat4 BoneTransform = bones[BoneIDs[0]] * Weights[0];
    BoneTransform     += bones[BoneIDs[1]] * Weights[1];
    BoneTransform     += bones[BoneIDs[2]] * Weights[2];
    BoneTransform     += bones[BoneIDs[3]] * Weights[3];

    gl_Position		= ViewProj * ModelMatrix * BoneTransform * Position;	
    FragTexture		= TexCoord;
}