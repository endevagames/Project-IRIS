#version 410 core

layout (location = 3) out vec4 gFinalColor;

in vec3 localPos;
in vec4 FragPos;

uniform samplerCube skybox;
  
void main()
{
    vec3 envColor = texture(skybox, localPos).rgb;
    
    envColor = envColor / (envColor + vec3(1.0));
    envColor = pow(envColor, vec3(1.0/2.2)); 
    
    gFinalColor = vec4(envColor, 1.0);
}