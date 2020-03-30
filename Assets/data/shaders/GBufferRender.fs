#version 440 core

#define NORMAL_MAP

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec2 TexCoords;
in vec3 FragPos;
#ifdef NORMAL_MAP
in mat3 TBN;
#else
in vec3 Normal;
#endif

struct Material 
{
    sampler2D             albedoMap;
    sampler2D  metallicRoughnessMap;
#ifdef NORMAL_MAP
    sampler2D             normalMap;
#endif
    sampler2D                 aoMap;
}; 

uniform Material material;

void main()
{    
    // Get the PBR components
    vec3 albedo     = pow(texture(material.albedoMap, TexCoords).rgb, vec3(2.2));
    float metallic  = texture(material.metallicRoughnessMap, TexCoords).b;
    float roughness = texture(material.metallicRoughnessMap, TexCoords).g;
    float ao        = texture(material.metallicRoughnessMap, TexCoords).r;

    // Store the fragment position vector in the first gbuffer texture
    gPosition.rgb = FragPos;

    // Store the ao component in the alpha value of gPosition
    gPosition.a = ao;

    // Also store the per-fragment normals into the gbuffer
#ifdef NORMAL_MAP
    gNormal.rgb = normalize(texture(material.normalMap, TexCoords).rgb * 2.0 - 1.0);
    gNormal.rgb = normalize(TBN * gNormal.rgb);
#else
    gNormal.rgb = Normal;
#endif

    // Store the roughness in the alpha value of gNormal
    gNormal.a = roughness;

    // And the diffuse per-fragment color
    gAlbedoSpec.rgb = albedo;

    // Store metallic value in gAlbedoSpec's alpha component
    gAlbedoSpec.a = metallic;
}