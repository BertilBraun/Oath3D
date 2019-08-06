
uniform sampler2D DisplacementTexture;
uniform vec3 EyePos;

vec2 ParallaxMapping(vec2 texCoords, vec3 pos)
{
	vec3 viewDir = EyePos - pos;
	
    float numLayers = mix(32, 8, abs(dot(vec3(0, 0, 1), viewDir)));
    float layerDepth = 1.0 / numLayers;
    float currentLayerDepth = 0.0;
    
	vec2 P = viewDir.xy / viewDir.z * 0.1; 
    vec2 deltaTexCoords = P / numLayers;
    vec2 currentTexCoords = texCoords;
	
    float currentDepthMapValue = texture(DisplacementTexture, currentTexCoords).r;
      
    while(currentLayerDepth < currentDepthMapValue) {
        currentTexCoords -= deltaTexCoords;
        currentDepthMapValue = texture(DisplacementTexture, currentTexCoords).r;
        currentLayerDepth += layerDepth;
    }
    
    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

    float afterDepth  = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = texture(DisplacementTexture, prevTexCoords).r - currentLayerDepth + layerDepth;
 
    float weight = afterDepth / (afterDepth - beforeDepth);

    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);
	
	if(texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0)
        discard;
	
	return finalTexCoords;
}