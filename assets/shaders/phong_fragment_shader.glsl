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

uniform vec3 viewPos;

uniform SpotLight spotLight;

uniform int numPointLights;
uniform PointLight pointLights[10];

uniform mat4 model; // si se necesita

void main()
{
    // Este fragment shader debería usar las variables definidas, o al menos alguna para no ser optimizadas.
    // Simulación simple de iluminación difusa:
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = vec3(0.0);

    // Ejemplo de uso, al menos con una pointLight si existe:
    if(numPointLights > 0) {
        // Calculo simple con la primera luz:
        vec3 lightDir = normalize(pointLights[0].position - FragPos);
        float diff = max(dot(norm, lightDir),0.0);
        vec3 diffuse = pointLights[0].diffuse * diff * texture(texture_diffuse1, TexCoords).rgb;
        result += diffuse;
    }

    // Uso del spotlight (ejemplo):
    {
        vec3 lightDir = normalize(spotLight.position - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = spotLight.diffuse * diff * texture(texture_diffuse1, TexCoords).rgb;
        result += diffuse;
    }

    FragColor = vec4(result,1.0);
}
