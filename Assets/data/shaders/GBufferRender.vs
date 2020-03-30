#version 440 core

#define NORMAL_MAP
#define MAX_BONES 100

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;
#ifdef ANIMATED_MESH
layout (location = 5) in uvec4 BoneIDs;
layout (location = 6) in vec4 Weights;
#endif

out vec3 FragPos;
out vec2 TexCoords;
#ifdef NORMAL_MAP
out mat3 TBN;
#else
out vec3 Normal;
#endif

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
#ifdef ANIMATED_MESH
uniform mat4 gBones[MAX_BONES];
#endif

void main()
{
    vec4 PosL = vec4(aPos, 1.0f);

#ifdef ANIMATED_MESH
    mat4 BoneTransform = gBones[BoneIDs[0]] * Weights[0];
    BoneTransform     += gBones[BoneIDs[1]] * Weights[1];
    BoneTransform     += gBones[BoneIDs[2]] * Weights[2];
    BoneTransform     += gBones[BoneIDs[3]] * Weights[3];
    PosL               = BoneTransform * vec4(aPos, 1.0f);
#endif
    FragPos = vec3(model * PosL);
    TexCoords = aTexCoords;
#ifdef NORMAL_MAP
    #ifdef ANIMATED_MESH
        vec3 T = normalize(vec3(model * BoneTransform * vec4(aTangent, 0.0)));
        vec3 N = normalize(vec3(model * BoneTransform * vec4(aNormal, 0.0)));
    #else
        vec3 T = normalize(vec3(model * vec4(aTangent, 0.0)));
        vec3 N = normalize(vec3(model * vec4(aNormal, 0.0)));
    #endif
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    TBN = mat3(T, B, N);
#else
    #ifdef ANIMATED_MESH
        Normal = transpose(inverse(mat3(model * BoneTransform))) * aNormal;
    #else
        Normal = transpose(inverse(mat3(model))) * aNormal;
    #endif
#endif

    gl_Position = projection * view * model * PosL;

}