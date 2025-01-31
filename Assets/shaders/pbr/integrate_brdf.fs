#version 430 core
out vec4 FragColor;
in vec2 TexCoords;

#include ../common/constants.glsl
#include ../common/sampling.glsl
#include ../common/brdf.glsl

vec2 IntegrateBRDF(float roughness, float NdotV)
{
    vec3 V;
    V.x = sqrt(1.0 - NdotV*NdotV);
    V.y = 0.0;
    V.z = NdotV;

    float A = 0.0;
    float B = 0.0;

    const uint SAMPLE_COUNT = 1024u;

    vec3 N = vec3(0.0, 0.0, 1.0);

    for(uint i = 0u; i < SAMPLE_COUNT; ++i)
    {
        // NOTE(Joey): generates a sample vector that's biased towards the
        // preferred alignment direction (importance sampling).
        vec2 Xi = Hammersley(i, SAMPLE_COUNT);
        vec3 H = ImportanceSampleGGX(Xi, N, roughness);
        vec3 L = normalize(2.0 * dot(V, H) * H - V);

        float NdotL = max(L.z, 0.0);
        float NdotH = max(H.z, 0.0);
        float VdotH = max(dot(V, H), 0.0);

        if(NdotL > 0.0)
        {
            float G = GeometryGGXIBL(NdotV, NdotL, roughness);
            float G_Vis = (G * VdotH) / (NdotH * NdotV);
            float Fc = pow(1.0 - VdotH, 5.0);

            A += (1.0 - Fc) * G_Vis;
            B += Fc * G_Vis;
        }
    }
    A /= float(SAMPLE_COUNT);
    B /= float(SAMPLE_COUNT);
    return vec2(A, B);
}

void main(void) 
{
    vec2 integratedBRDF = IntegrateBRDF(TexCoords.y, TexCoords.x);
    FragColor = vec4(integratedBRDF, 0.0, 1.0);
}