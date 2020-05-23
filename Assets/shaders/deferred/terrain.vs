#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV0;
layout (location = 2) in vec3 aNormal;

out vec2 UV0;
out vec3 FragPos;
out vec3 Normal;
out vec4 ClipSpacePos;
out vec4 PrevClipSpacePos;

#include ../common/uniforms.glsl

uniform mat4 model;
uniform mat4 prevModel;

float time;

void main()
{
	UV0 = aUV0;
	FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = aNormal;

    ClipSpacePos     = viewProjection * model * vec4(aPos, 1.0);
    PrevClipSpacePos = prevViewProjection * prevModel * vec4(aPos, 1.0);
	
	gl_Position =  projection * view * vec4(FragPos, 1.0);
}