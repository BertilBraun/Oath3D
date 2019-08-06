
struct BaseLight
{
    vec3 Color;
    float AmbientIntensity;
    float DiffuseIntensity;
};

struct Attenuation
{
    float Constant;
    float Linear;
    float Exp;
};

struct PointLight
{
    BaseLight Base;
    vec3 Position;
    Attenuation Atten;
};

struct SpotLight
{
    PointLight Base;
    vec3 Direction;
    float Cutoff;
};

struct DirectionalLight
{
    BaseLight Base;
    vec3 Direction;
};

uniform vec3 EyePos;
uniform vec2 ScreenSize;

float SpecularIntensity;
float SpecularPower;

vec4 CalcLightInternal(BaseLight Light, vec3 LightDirection, vec3 WorldPos, vec3 Normal)
{
	if (length(WorldPos) >= 500) //SKYBOX
		return vec4(1);

    vec4 AmbientColor = vec4(Light.Color * Light.AmbientIntensity, 1.0);
    float DiffuseFactor = dot(Normal, -LightDirection);

    vec4 DiffuseColor  = vec4(0);
    vec4 SpecularColor = vec4(0);

    if (DiffuseFactor > 0.0) {
        DiffuseColor = vec4(Light.Color * Light.DiffuseIntensity * DiffuseFactor, 1.0);

        vec3 VertexToEye = normalize(EyePos - WorldPos);
        vec3 LightReflect = normalize(reflect(LightDirection, Normal));
        float SpecularFactor = dot(VertexToEye, LightReflect);        
        if (SpecularFactor > 0.0) {
            SpecularFactor = pow(SpecularFactor, SpecularPower);
            SpecularColor = vec4(Light.Color * SpecularIntensity * SpecularFactor, 1.0);
        }
    }

    return (AmbientColor + DiffuseColor + SpecularColor);
}

vec4 CalcPointLight(PointLight pl, vec3 WorldPos, vec3 Normal)
{
    vec3 LightDirection = WorldPos - pl.Position;
    float Distance = length(LightDirection);

    vec4 Color = CalcLightInternal(pl.Base, normalize(LightDirection), WorldPos, Normal);

    float Attenuation =  pl.Atten.Constant +
                         pl.Atten.Linear * Distance +
                         pl.Atten.Exp * Distance * Distance;

    return Color / max(1.0, Attenuation);
}

vec4 CalcSpotLight(SpotLight sl, vec3 WorldPos, vec3 Normal) 
{
    vec3 LightToPixel = normalize(WorldPos - sl.Base.Position);
    float SpotFactor = dot(LightToPixel, sl.Direction);
	
    if (SpotFactor > sl.Cutoff) {
        vec4 Color = CalcPointLight(sl.Base, sl.Base.Position, Normal);
        return Color * (1.0 - (1.0 - SpotFactor) * 1.0/(1.0 - sl.Cutoff));
    }
    else 
        return vec4(0);
}

vec4 CalcDirectionalLight(DirectionalLight dl, vec3 WorldPos, vec3 Normal)
{
    return CalcLightInternal(dl.Base, normalize(dl.Direction), WorldPos, Normal);
}

vec2 CalcTexCoord()
{
    return gl_FragCoord.xy / ScreenSize;
}
