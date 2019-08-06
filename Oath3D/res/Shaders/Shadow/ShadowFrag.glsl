#version 330

const int NUM_CASCADES = 1;

uniform sampler2D FinalTexture;
uniform sampler2D ShadowTexture[NUM_CASCADES];
uniform sampler2D PositionTexture;
uniform sampler2D NormalTexture;

uniform vec3 LightPos;
uniform vec3 AmbientFactor;

uniform float EndClipSpace[NUM_CASCADES];
uniform mat4 LightViewProj[NUM_CASCADES];
uniform mat4 ViewProj;

in vec2 FragTexture;
out vec4 outColor;

float ShadowCalculation(int CascadeIndex, vec4 fragPosLightSpace, vec4 Normal, vec4 Position)
{
    vec3 projCoords = (fragPosLightSpace.xyz / fragPosLightSpace.w) * 0.5 + 0.5;
	
    float closestDepth = texture(ShadowTexture[CascadeIndex], projCoords.xy).r;
    float currentDepth = projCoords.z;
	
	if (currentDepth > 1.0)
		return 1.0;
	
    float bias 		= max(0.05 * (1.0 - dot(normalize(Normal.xyz), normalize(LightPos - Position.xyz))), 0.005);	
	
    vec2 texelSize 	= 1.0 / textureSize(ShadowTexture[CascadeIndex], 0);
	
    float shadow 	= 0.0;
	
    for (int x = -2; x <= 2; x++)
        for (int y = -2; y <= 2; y++)
			if (currentDepth - bias < texture(ShadowTexture[CascadeIndex], projCoords.xy + vec2(x, y) * texelSize).r)
				shadow += 1.0;

    return shadow / 25.0;
}

void main() {

	vec4 color		= texture(FinalTexture,		FragTexture);
	vec4 position	= texture(PositionTexture, 	FragTexture);
	vec4 normal		= texture(NormalTexture, 	FragTexture);
	
	float shadow = 1;
	
	for (int i = 0; i < NUM_CASCADES; i++)
		if ((ViewProj * position).z  < EndClipSpace[i]) {
		
			vec4 fragPosLightSpace = LightViewProj[i] * position;
			
			shadow = ShadowCalculation(i, fragPosLightSpace, normal, position);
			
            break;
        }

	outColor = color * (vec4(AmbientFactor, 1) + shadow);
	//outColor = color;
	//outColor = texture(ShadowTexture[0], FragTexture);
	//outColor = normal;

	//outColor = vec4(vec3(shadow), 1);
	//outColor = Proj * position;
	//outColor = vec4(vec3(EndClipSpace[1]), 1);
	//outColor = LightViewProj[0] * position;

	//outColor = lightSpacePos;
	//outColor = vec4(texture(ShadowTexture, FragTexture).xyz, 1);
}