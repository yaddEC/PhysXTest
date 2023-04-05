#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

flat out vec4 color;

out vec2 TexCoord;

vec3 Normal;
vec3 FragPos;

uniform mat4 mvp;
uniform mat4 model;

uniform vec3 viewPos;

// Structs

struct DirLight {
    int isActive;
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  

#define MAX_DIR_LIGHTS 2
uniform DirLight dirLights[MAX_DIR_LIGHTS];
uniform int maxDirLights;

struct PointLight {
    int isActive;
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};
#define MAX_POINT_LIGHTS 10
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform int maxPointLights;

struct SpotLight { 
    int isActive;
    vec3  position;
    vec3  direction;

    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
}; 

#define MAX_SPOT_LIGHTS 2
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];
uniform int maxSpotLights;

// Fuctions

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

    vec3 ambient  = light.ambient;
    vec3 diffuse  = light.diffuse  * diff;
    vec3 specular = light.specular * spec;
    return (ambient + diffuse + specular);
}  

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 ambient = light.ambient;
    vec3 diffuse = light.diffuse * diff;
    vec3 specular = light.specular * spec;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight spotLight, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    
    vec3 lightDir = normalize(spotLight.position - fragPos);
    float theta = dot(lightDir, normalize(-spotLight.direction)); 
    float epsilon = spotLight.cutOff - spotLight.outerCutOff;
    float intensity = clamp((theta - spotLight.outerCutOff) / epsilon, 0.0, 1.0);
    
    if(theta > spotLight.outerCutOff) 
    {   
        float diff = max(dot(normal, lightDir), 0.0);

        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

        float distance = length(spotLight.position - fragPos);
        float attenuation = 1.0 / (spotLight.constant + spotLight.linear * distance + spotLight.quadratic * (distance * distance));

        vec3 diffuse  = spotLight.diffuse  * diff * intensity * attenuation;
        vec3 specular  = spotLight.specular * spec * intensity * attenuation;

        return (spotLight.ambient + diffuse + specular);  
    }
    return spotLight.ambient;
}

void main()
{
	gl_Position = mvp * vec4(aPos, 1.0);
	TexCoord = aTexCoord;
	
	Normal = vec3(model * vec4(aNormal, 0.0));
	FragPos = vec3(model * vec4(aPos, 1.0));

	vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = vec3(0);
    bool lightActive = false;

    //directional light
    for(int i = 0; i < maxDirLights; i++)
    {
        if(dirLights[i].isActive == 1)
        {
            result += CalcDirLight(dirLights[i], norm, viewDir);
            lightActive = true;
        }
    }

    //point lights
    for(int i = 0; i < maxPointLights; i++)
    {
        if(pointLights[i].isActive == 1)
        {
            result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
            lightActive = true;
        }
    }

    //spot light
    for(int i = 0; i < maxSpotLights; i++)
    {
        if(spotLights[i].isActive == 1)
        {
            result += CalcSpotLight(spotLights[i], norm, FragPos, viewDir);
            lightActive = true;
        }
    }

    color = vec4(1,1,1,1);

    if (lightActive)
    {
        color = vec4(result, 1.0);
    }
}

