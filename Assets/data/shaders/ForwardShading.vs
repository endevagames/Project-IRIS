#version 410 core

layout (location = 0) in vec3 aPos; // Vertex positions
layout (location = 1) in vec3 aNormal; // Normals
layout (location = 2) in vec2 aTexCoords; // Optional Texture coordinates 
layout (location = 3) in vec3 aTangent; // Optional Texture coordinates 
layout (location = 4) in vec3 aBitangent; // Optional Texture coordinates 

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
out vec4 FragPosLightSpace;
out mat3 TBN;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;  
    TexCoords = aTexCoords;
    FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);

    vec3 T = normalize(vec3(model * vec4(aTangent, 0.0)));
    vec3 N = normalize(vec3(model * vec4(aNormal, 0.0)));
    
    // re-orthogonalize T with respect to N
    T = normalize(T - dot(T, N) * N);

    // then retrieve perpendicular vector B with the cross product of T and N
    vec3 B = cross(N, T);
    mat3 TBN = transpose(mat3(T, B, N));

    gl_Position = projection * view * vec4(FragPos, 1.0);
}