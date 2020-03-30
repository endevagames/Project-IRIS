#version 410 core

layout (location = 3) out vec4 gFinalColor;
  
in vec2 TexCoords;

struct DirLight 
{
    vec3 color;
    vec3 position;
    vec3 direction;
    mat4 lightSpaceMatrix;
    int shadowMap;
};

struct PointLight 
{
    vec3 color;
    vec3 position;    
    float constant;
    float linear;
    float quadratic;
    int shadowMap;
};

// Constants
const int N_DIR_LIGHTS   = 2;
const int N_POINT_LIGHTS = 2;
const float PI           = 3.14159265359;

// Geometry buffer textures
uniform sampler2D   gPosition;
uniform sampler2D   gNormal;
uniform sampler2D   gAlbedoSpec;

// Image based lighting
uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D   brdfLUT;  

// Lights
uniform DirLight   dirLights[N_DIR_LIGHTS];
uniform PointLight pointLights[N_POINT_LIGHTS];

// Shadow Maps
uniform sampler2D     dirLightShadowMaps[N_DIR_LIGHTS];
uniform samplerCube   pointShadowMaps[N_POINT_LIGHTS];

uniform vec3 viewPos;
uniform float far_plane;

// PBR calculation methods
float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0);
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness);

// Light calculation methods
vec3 CalcDirLight(DirLight light, vec3 N, vec3 V, vec3 WorldPos, vec3 albedo, float roughness, float metallic, vec3 F0);
vec3 CalcPointLight(PointLight light, vec3 N, vec3 V, vec3 WorldPos, vec3 albedo, float roughness, float metallic, vec3 F0);

// Shadow calculation methods
float DirectionalShadowCalculation(sampler2D shadowMap, vec4 fragPosLightSpace, float bias);
float PointShadowCalculation(samplerCube shadowCubeMap, float viewDistance, vec3 fragToLight, float bias);

void main()
{             
    // retrieve data from G-buffer
    vec3 WorldPos    = texture(gPosition, TexCoords).rgb;
    vec3 albedo      = texture(gAlbedoSpec, TexCoords).rgb;
    vec3 N           = texture(gNormal, TexCoords).rgb;
    float ao         = texture(gPosition, TexCoords).a;
    float metallic   = texture(gAlbedoSpec, TexCoords).a;
    float roughness  = texture(gNormal, TexCoords).a;
    
    // then calculate lighting as usual
    vec3 V = normalize(viewPos - WorldPos);
    vec3 H = normalize(texture(brdfLUT, vec2(max(dot(N, V), 0.0), roughness)).rgb + V);
    vec3 R = reflect(-V, N); 

    vec3 result = vec3(0.0f);

    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);
    
    for (int i = 0; i < N_DIR_LIGHTS; i++)
        result += CalcDirLight(dirLights[i], N, V, WorldPos, albedo, roughness, metallic, F0);

    for (int i = 0; i < N_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], N, V, WorldPos, albedo, roughness, metallic, F0);
    
    // ambient lighting
    vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);

    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;	  
    
    vec3 irradiance = texture(irradianceMap, N).rgb;
    vec3 diffuse    = irradiance * albedo;

    const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefilteredColor = textureLod(prefilterMap, R,  roughness * MAX_REFLECTION_LOD).rgb;   
    vec2 envBRDF  = texture(brdfLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
    vec3 specular = prefilteredColor * (F * envBRDF.x + envBRDF.y);
    
    vec3 ambient = (kD * diffuse + specular) * ao;   
    
    vec3 color = ambient + result;

    // HDR tonemapping
    color = color / (color + vec3(1.0));
    
    // gamma correct
    color = pow(color, vec3(1.0f/2.2f)); 

    gFinalColor = vec4(color, 1.0f);
}  

/************************************************************************

                            Light calculation

*************************************************************************/

vec3 CalcPointLight(PointLight light, vec3 N, vec3 V, vec3 WorldPos, vec3 albedo, float roughness, float metallic, vec3 F0)
{
    // calculate per-light radiance
    vec3 L = normalize(light.position - WorldPos);
    float distance = length(light.position - WorldPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
    vec3 radiance = light.color * attenuation;

    // Cook-Torrance BRDF
    float NDF = DistributionGGX(N, V, roughness);   
    float G   = GeometrySmith(N, V, L, roughness);      
    vec3 F = fresnelSchlick(max(dot(N, V), 0.0), F0); 
           
    vec3 nominator    = NDF * G * F; 
    float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
    vec3 specular = nominator / max(denominator, 0.001); // prevent divide by zero for NdotV=0.0 or NdotL=0.0
        
    // kS is equal to Fresnel
    vec3 kS = F;
    // for energy conservation, the diffuse and specular light can't
    // be above 1.0 (unless the surface emits light); to preserve this
    // relationship the diffuse component (kD) should equal 1.0 - kS.
    vec3 kD = vec3(1.0) - kS;
    // multiply kD by the inverse metalness such that only non-metals 
    // have diffuse lighting, or a linear blend if partly metal (pure metals
    // have no diffuse light).
    kD *= 1.0 - metallic;	  

    // scale light by NdotL
    float NdotL = max(dot(N, L), 0.0);                   

        // get vector between fragment position and light position
    vec3 fragToLight = WorldPos - light.position;
    float viewDistance = length(V-WorldPos);

    float bias = max(0.05 * (1.0 - dot(N, L)), 0.005);  
    float shadow = PointShadowCalculation(pointShadowMaps[light.shadowMap], viewDistance, fragToLight, bias); 

    return (albedo * kD / PI + specular) * radiance * (1.0f - shadow) * NdotL; 
}

vec3 CalcDirLight(DirLight light, vec3 N, vec3 V, vec3 WorldPos, vec3 albedo, float roughness, float metallic, vec3 F0)
{
    // calculate per-light radiance
    vec3 L = normalize(light.position - WorldPos);
    vec3 radiance = light.color;

    // Cook-Torrance BRDF
    float NDF = DistributionGGX(N, V, roughness);   
    float G   = GeometrySmith(N, V, L, roughness);      
    vec3 F = fresnelSchlick(max(dot(N, V), 0.0), F0); 
           
    vec3 nominator    = NDF * G * F; 
    float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
    vec3 specular = nominator / max(denominator, 0.001); // prevent divide by zero for NdotV=0.0 or NdotL=0.0
        
    // kS is equal to Fresnel
    vec3 kS = F;
    // for energy conservation, the diffuse and specular light can't
    // be above 1.0 (unless the surface emits light); to preserve this
    // relationship the diffuse component (kD) should equal 1.0 - kS.
    vec3 kD = vec3(1.0) - kS;
    // multiply kD by the inverse metalness such that only non-metals 
    // have diffuse lighting, or a linear blend if partly metal (pure metals
    // have no diffuse light).
    kD *= 1.0 - metallic;	  

    vec4 fragPosLightSpace = light.lightSpaceMatrix * vec4(WorldPos, 1.0);
    float bias = max(0.01 * (1.0 - dot(N, L)), 0.001); 
    float shadow = DirectionalShadowCalculation(dirLightShadowMaps[light.shadowMap], fragPosLightSpace, bias);

    // scale light by NdotL
    float NdotL = max(dot(N, L), 0.0);                   
    return (kD * albedo / PI + specular) * (1.0 - shadow) * radiance * NdotL; 
}

/************************************************************************

                            Shadow calculation

*************************************************************************/

float DirectionalShadowCalculation(sampler2D shadowMap, vec4 fragPosLightSpace, float bias)
{ 
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    // If the z of the projected cordinates is greater than one, just return 0 (no shadow)
    if(projCoords.z > 1.0)
        return 0.0;

    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    
    // check whether current frag pos is in shadow. Sample the surrounding region to get smooth shadows
    float shadow = 0.0;
    vec2 texelSize = 0.25 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;

    return shadow;
}

float PointShadowCalculation(samplerCube shadowCubeMap, float viewDistance, vec3 fragToLight, float bias)
{
    // now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);

    float shadow  = 0.0;
    float offset  = 0.1;

    vec3 sampleOffsetDirections[20] = vec3[]
    (
        vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
        vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
        vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
        vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
        vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
    );   

    int samples  = 20;
    float diskRadius = (1.0 + (viewDistance / far_plane)) / 25.0;  
    for(int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(shadowCubeMap, fragToLight + sampleOffsetDirections[i] * diskRadius).r;
        closestDepth *= far_plane;   // Undo mapping [0;1]
        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(samples);  

    return shadow;
}  

/************************************************************************

                            PBR calculation

*************************************************************************/

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0f - F0) * pow(1.0f - cosTheta, 5.0f);
}  

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}   