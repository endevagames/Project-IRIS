#version 410 core

out vec4 FragColor;

struct Material {
    sampler2D diffuseMap;
    sampler2D specularMap;
    sampler2D normalMap;
    float shininess;
    vec3 ambientColour;
    vec3 diffuseColour;
    vec3 specularColour;
}; 

struct DirLight {
    vec3 direction;
	vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

#define NR_POINT_LIGHTS 4

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec4 FragPosLightSpace;
in mat3 TBN;

uniform Material material;

uniform vec3 viewPos;
uniform bool hasNormalMap;

uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;

uniform sampler2D shadowMap;
uniform samplerCube shadowCubeMap;

uniform float far_plane;

// function prototypes
float PointShadowCalculation(float viewDistance, vec3 fragPos, float bias);
float DirectionalShadowCalculation(vec4 fragPosLightSpace, float bias);
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{    
    // obtain normal from normal map in range [0,1]
    vec3 normal = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    if(hasNormalMap)
    {
        // Calculate normal and viewDir from normal map and tangent space
        normal = texture(material.normalMap, TexCoords).rgb;
        normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space
        viewDir = TBN * normalize(viewPos - FragPos);
    }
    
    // == =====================================================
    // Our lighting is set up in 3 phases: directional, point lights and an optional flashlight
    // For each phase, a calculate function is defined that calculates the corresponding color
    // per lamp. In the main() function we take all the calculated colors and sum them up for
    // this fragment's final color.
    // == =====================================================
    // phase 1: directional lighting
    vec3 result = CalcDirLight(dirLight, normal, viewDir);
    // phase 2: point lights
    //for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[0], normal, FragPos, viewDir);    
    // phase 3: spot light
    // result += CalcSpotLight(spotLight, norm, FragPos, viewDir);    
    
    FragColor = vec4(result, 1.0);
}

// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - FragPos);

    if (hasNormalMap) 
        lightDir = TBN * normalize(light.position - FragPos);

    vec3 color = texture(material.diffuseMap, TexCoords).rgb;
    vec3 halfwayDir = normalize(lightDir + viewDir);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    // combine results
    vec3 ambient = light.ambient * color;
    vec3 diffuse = light.diffuse * diff;
    vec3 specular = light.specular * spec;

    float bias = max(0.01 * (1.0 - dot(normal, lightDir)), 0.001); 
    float shadow = DirectionalShadowCalculation(FragPosLightSpace, bias); 
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;

    return lighting;
}

// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir   = TBN * normalize(light.position - fragPos);

    if (hasNormalMap) 
        lightDir = TBN * normalize(light.position - FragPos);

    vec3 color = texture(material.diffuseMap, TexCoords).rgb;

    vec3 halfwayDir = normalize(lightDir + viewDir);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // combine results
    vec3 ambient = light.ambient * color;
    vec3 diffuse = light.diffuse * diff;
    vec3 specular = light.specular * spec;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    // get vector between fragment position and light position
    vec3 fragToLight = fragPos - light.position;
    float viewDistance = length(viewPos-fragPos);

    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);  
    float shadow = PointShadowCalculation(viewDistance, fragToLight, bias); 
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;

    return lighting;
}

// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir   = TBN * normalize(light.position - fragPos);

    if (hasNormalMap) 
        lightDir = TBN * normalize(light.position - FragPos);

    vec3 halfwayDir = normalize(lightDir + viewDir);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * (vec3(texture(material.diffuseMap, TexCoords)) + material.ambientColour);
    vec3 diffuse = light.diffuse * diff * (vec3(texture(material.diffuseMap, TexCoords)) + material.diffuseColour);
    vec3 specular = light.specular * spec * (vec3(texture(material.specularMap, TexCoords)) + material.specularColour);

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}

float DirectionalShadowCalculation(vec4 fragPosLightSpace, float bias)
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

float PointShadowCalculation(float viewDistance, vec3 fragToLight, float bias)
{
    // use the light to fragment vector to sample from the depth map    
    float closestDepth = texture(shadowCubeMap, fragToLight).r;
    // it is currently in linear range between [0,1]. Re-transform back to original value
    closestDepth *= far_plane;
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