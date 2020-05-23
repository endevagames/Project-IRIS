#version 430 core
layout (location = 0) out vec4 gPositionMetallic;
layout (location = 1) out vec4 gNormalRoughness;
layout (location = 2) out vec4 gAlbedoAO;
layout (location = 3) out vec4 gMotion; // TODO: for ease of use with development; package g-buffer properties (linear depth instead of position, normal as xz pair etc.)

in vec2 UV0;
in vec3 FragPos;
in vec3 Normal;
in vec4 ClipSpacePos;
in vec4 PrevClipSpacePos;

uniform sampler2D TexAlbedo;

void main()
{
    // store the fragment position vector in the first gbuffer texture
    gPositionMetallic.rgb = FragPos;
    gPositionMetallic.a = 0.0f;

    // also store the per-fragment (bump-)normals into the gbuffer;
    gNormalRoughness.rgb = normalize(Normal * 2.0 - 1.0);
    gNormalRoughness.a = 1.0f;

    // and the diffuse per-fragment color
    gAlbedoAO.rgb = texture(TexAlbedo, UV0).rgb;
    gAlbedoAO.a = 0.8f;

    // per-fragment motion vector
    vec2 clipSpace = ClipSpacePos.xy / ClipSpacePos.w;
    vec2 prevClipSpace = PrevClipSpacePos.xy / PrevClipSpacePos.w;
    gMotion.xy = clipSpace - prevClipSpace;
}