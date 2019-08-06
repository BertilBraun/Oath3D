#version 330

uniform sampler2D FinalTexture;
uniform sampler2D PositionTexture;
uniform sampler2D DepthTexture;
uniform sampler2D NormalTexture;
uniform sampler2D SpecularTexture;

in vec2 FragTexture;
out vec4 outColor;

uniform mat4 proj;
uniform mat4 view;

// Consts should help improve performance
const float rayStep = 0.25;
const float minRayStep = 0.1; //30 mby try
const float maxSteps = 20;
const float searchDist = 5;
const float searchDistInv = 0.2;
const int numBinarySearchSteps = 5;
const float maxDDepth = 1.0;
const float maxDDepthInv = 1.0;

const float reflectionSpecularFalloffExponent = 3.0;

vec3 BinarySearch(vec3 dir, inout vec3 hitCoord, out float dDepth)
{
    float depth;

    for(int i = 0; i < numBinarySearchSteps; i++)
    {
        vec4 projectedCoord = proj * vec4(hitCoord, 1.0);
        projectedCoord.xy /= projectedCoord.w;
        projectedCoord.xy = projectedCoord.xy * 0.5 + 0.5;

        depth = texture(PositionTexture, projectedCoord.xy).z;

        dDepth = hitCoord.z - depth;

        if(dDepth > 0.0)
            hitCoord += dir;

        dir *= 0.5;
        hitCoord -= dir;    
    }

    vec4 projectedCoord = proj * vec4(hitCoord, 1.0); 
    projectedCoord.xy /= projectedCoord.w;
    projectedCoord.xy = projectedCoord.xy * 0.5 + 0.5;

    return vec3(projectedCoord.xy, depth);
}

vec4 RayCast(vec3 dir, inout vec3 hitCoord, out float dDepth)
{
    dir *= rayStep;

    float depth;

    for(int i = 0; i < maxSteps; i++)
    {
        hitCoord += dir;


        vec4 projectedCoord = proj * vec4(hitCoord, 1.0);
        projectedCoord.xy /= projectedCoord.w;
        projectedCoord.xy = projectedCoord.xy * 0.5 + 0.5;

        depth = texture(PositionTexture, projectedCoord.xy).z;

        dDepth = hitCoord.z - depth;

        if(dDepth < 0.0)
            return vec4(BinarySearch(dir, hitCoord, dDepth), 1.0);
    }

    return vec4(0.0);
}

void main()
{
    // Samples
    float specular = texture(SpecularTexture, FragTexture).g;

	specular = 1;

    if(specular == 0.0)
    {
        outColor = vec4(0.0);
        return;
    }

    vec4 viewNormal = /*view * */ texture(NormalTexture, 	FragTexture);
    vec4 viewPos 	= /*view * */ texture(PositionTexture, 	FragTexture);

    // Reflection vector
    vec3 reflected = normalize(reflect(normalize(viewPos.xyz), normalize(viewNormal.xyz)));

    // Ray cast
    vec3 hitPos = viewPos.xyz;
    float dDepth;

    vec4 coords = RayCast(reflected * max(minRayStep, -viewPos.z), hitPos, dDepth);

    vec2 dCoords = abs(vec2(0.5, 0.5) - coords.xy);

    float screenEdgefactor = clamp(1.0 - (dCoords.x + dCoords.y), 0.0, 1.0);

	float alpha = 	pow(specular, reflectionSpecularFalloffExponent) *
					screenEdgefactor * clamp(-reflected.z, 0.0, 1.0) *
					clamp((searchDist - length(viewPos.xyz - hitPos)) * searchDistInv, 0.0, 1.0) * coords.w;

    // Get color
    outColor = vec4(texture(FinalTexture, coords.xy).rgb, alpha);
	
    //outColor = texture(FinalTexture, FragTexture);
    //outColor = texture(PositionTexture, FragTexture);
    //outColor = texture(NormalTexture, FragTexture);
    //outColor = texture(SpecularTexture, FragTexture);
	
    outColor = viewNormal;
   // outColor = viewPos;
	
    //outColor = vec4(alpha);
}





//#version 330
//
//uniform sampler2D FinalTexture;
//uniform sampler2D PositionTexture;
//uniform sampler2D NormalTexture;
//uniform sampler2D SpecularTexture;
//
//uniform mat4 view;
//uniform mat4 invView;
//
//uniform mat4 proj;
//uniform mat4 invProj;
//
//in vec2 FragTexture;
//
//out vec4 outColor;
//
//const float rayStep = 0.05;
//const float minRayStep = 0.2;
//const float maxSteps = 30;
//const int numBinarySearchSteps = 10;
//const float reflectionSpecularFalloffExponent = 3.0;
//
//#define Scale vec3(.8, .8, .8)
//#define K 19.19
//
//vec3 PositionFromDepth(float depth);
//
//vec3 BinarySearch(inout vec3 dir, inout vec3 hitCoord, inout float dDepth);
// 
//vec4 RayMarch(vec3 dir, inout vec3 hitCoord, out float dDepth);
//
//vec3 fresnelSchlick(float cosTheta, vec3 F0);
//
//vec3 hash(vec3 a);
//
//void main()
//{
//
//    vec2 MetallicEmmissive = texture2D(SpecularTexture, FragTexture).rg;
//    float Metallic = MetallicEmmissive.r;
//    Metallic = 0.9;
//
//    if(Metallic < 0.01)
//        discard;
// 
//    vec3 viewNormal = vec3(texture2D(NormalTexture, FragTexture) * invView);
//    vec3 viewPos = textureLod(PositionTexture, FragTexture, 2).xyz;
//    vec3 albedo = texture(FinalTexture, FragTexture).rgb;
//
//    float spec = texture(SpecularTexture, FragTexture).b;
//    spec = 0.9;
//
//    vec3 F0 = vec3(0.04); 
//    F0      = mix(F0, albedo, Metallic);
//    vec3 Fresnel = fresnelSchlick(max(dot(normalize(viewNormal), normalize(viewPos)), 0.0), F0);
//
//    // Reflection vector
//    vec3 reflected = normalize(reflect(normalize(viewPos), normalize(viewNormal)));
//
//
//    vec3 hitPos = viewPos;
//    float dDepth;
// 
//    vec3 wp = vec3(vec4(viewPos, 1.0) * invView);
//    vec3 jitt = mix(vec3(0.0), vec3(hash(wp)), spec);
//    vec4 coords = RayMarch((vec3(jitt) + reflected * max(minRayStep, -viewPos.z)), hitPos, dDepth);
// 
// 
//    vec2 dCoords = smoothstep(0.2, 0.6, abs(vec2(0.5, 0.5) - coords.xy));
// 
// 
//    float screenEdgefactor = clamp(1.0 - (dCoords.x + dCoords.y), 0.0, 1.0);
//
//    float ReflectionMultiplier = pow(Metallic, reflectionSpecularFalloffExponent) * 
//                screenEdgefactor * 
//                -reflected.z;
// 
//    // Get color
//    vec3 SSR = textureLod(FinalTexture, dCoords.xy, 0).rgb * clamp(ReflectionMultiplier, 0.0, 0.9) * Fresnel;  
//
//    outColor = vec4(SSR, 1/*Metallic*/);
//	
//	outColor = textureLod(FinalTexture, FragTexture.xy, 2) + vec4(SSR, Metallic);
//	//outColor = vec4(dCoords, 1, 1);
//	//outColor = coords;
//}
//
//vec3 PositionFromDepth(float depth) {
//
//    vec4 clipSpacePosition = vec4(FragTexture * 2.0 - 1.0, depth * 2.0 - 1.0, 1.0);
//    vec4 viewSpacePosition = invProj * clipSpacePosition;
//
//    return viewSpacePosition.xyz / viewSpacePosition.w;
//}
//
//vec3 BinarySearch(inout vec3 dir, inout vec3 hitCoord, inout float dDepth)
//{
//    float depth;
//
//    vec4 projectedCoord;
// 
//    for(int i = 0; i < numBinarySearchSteps; i++)
//    {
//
//        projectedCoord = proj * vec4(hitCoord, 1.0);
//        projectedCoord.xy /= projectedCoord.w;
//        projectedCoord.xy = projectedCoord.xy * 0.5 + 0.5;
// 
//        depth = textureLod(PositionTexture, projectedCoord.xy, 2).z;
//
// 
//        dDepth = hitCoord.z - depth;
//
//        dir *= 0.5;
//        if(dDepth > 0.0)
//            hitCoord += dir;
//        else
//            hitCoord -= dir;    
//    }
//
//	projectedCoord = proj * vec4(hitCoord, 1.0);
//	projectedCoord.xy /= projectedCoord.w;
//	projectedCoord.xy = projectedCoord.xy * 0.5 + 0.5;
//
//    return vec3(projectedCoord.xy, depth);
//}
//
//vec4 RayMarch(vec3 dir, inout vec3 hitCoord, out float dDepth)
//{
//    dir *= rayStep;
// 
//    float depth;
//    vec4 projectedCoord;
//
//    for(int i = 0; i < maxSteps; i++)
//    {
//        hitCoord += dir;
// 
//        projectedCoord = proj * vec4(hitCoord, 1.0);
//        projectedCoord.xy /= projectedCoord.w;
//        projectedCoord.xy = projectedCoord.xy * 0.5 + 0.5;
// 
//        depth = textureLod(PositionTexture, projectedCoord.xy, 2).z;
//        if(depth > 1000.0)
//            continue;
// 
//        dDepth = hitCoord.z - depth;
//
//        if ((dir.z - dDepth) < 1.2 && dDepth <= 0.0) 
//			return vec4(BinarySearch(dir, hitCoord, dDepth), 1.0);
//    }
// 
//    return vec4(projectedCoord.xy, depth, 0.0);
//}
//
//vec3 fresnelSchlick(float cosTheta, vec3 F0)
//{
//    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
//}
//
//
//vec3 hash(vec3 a)
//{
//    a = fract(a * Scale);
//    a += dot(a, a.yxz + K);
//    return fract((a.xxy + a.yxx)*a.zyx);
//}