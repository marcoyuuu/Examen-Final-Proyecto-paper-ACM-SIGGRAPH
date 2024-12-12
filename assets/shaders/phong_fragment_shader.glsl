#version 330 core

out vec4 FragColor;

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

in vec3 FragPos;  
in vec3 Normal;  
in vec3 Color;   
in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_roughness1;

// Camera position
uniform vec3 viewPos;

// Spotlight
uniform SpotLight spotLight;

// Multiple point lights
uniform int numPointLights;
uniform PointLight pointLights[10]; // Up to 10 point lights

// Calculate attenuation
float CalcAttenuation(PointLight light, float distance) {
    return 1.0 / (light.constant + light.linear * distance + 
                  light.quadratic * (distance * distance));
}

// Calculate point light contribution
vec3 CalcPointLight(PointLight light, vec3 norm, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);

    // Halfway vector for specular
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);

    float distance = length(light.position - fragPos);
    float attenuation = CalcAttenuation(light, distance);

    vec3 ambient = light.ambient * texture(texture_diffuse1, TexCoords).rgb;
    vec3 diffuse = light.diffuse * diff * texture(texture_diffuse1, TexCoords).rgb;
    vec3 specular = light.specular * spec;

    // Apply attenuation
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}

// Calculate spotlight contribution
vec3 CalcSpotLight(SpotLight light, vec3 norm, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);

    // Specular
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
                               light.quadratic * (distance * distance));

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff)/epsilon, 0.0, 1.0);

    vec3 ambient = light.ambient * texture(texture_diffuse1, TexCoords).rgb;
    vec3 diffuse = light.diffuse * diff * texture(texture_diffuse1, TexCoords).rgb;
    vec3 specular = light.specular * spec;

    // Apply spotlight intensity and attenuation
    diffuse *= intensity * attenuation;
    specular *= intensity * attenuation;
    ambient *= attenuation;

    return ambient + diffuse + specular;
}

void main()
{
    // Normal mapping if needed (currently just using Normal)
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // Start with black
    vec3 result = vec3(0.0);

    // Add spotlight contribution
    result += CalcSpotLight(spotLight, norm, FragPos, viewDir);

    // Add all point lights contribution
    for(int i = 0; i < numPointLights; i++) {
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    }

    FragColor = vec4(result, 1.0);
}
