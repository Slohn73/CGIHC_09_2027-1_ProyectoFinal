#version 330 core

#define NUMBER_OF_POINT_LIGHTS 4

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    bool hasDiffuseMap;
    bool hasSpecularMap;
    vec3 diffuseColor;
    vec3 specularColor;
    vec3 emissive;
    float shininess;
    float opacity;
    bool alphaTest;
};

struct DirLight
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
    vec3 position;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight
{
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

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec3 VertexColor; // Recibe el color por vértice del Vertex Shader

out vec4 color;

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLights[NUMBER_OF_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform Material material;
uniform int transparency;

// Prototipo de funciones
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 baseColor, vec3 specColor);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 baseColor, vec3 specColor);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 baseColor, vec3 specColor);

void main()
{
    // Obtener el color base multiplicando el color por vértice enviado desde Mesh.h
    vec4 texColor = material.hasDiffuseMap ? texture(material.diffuse, TexCoords) : vec4(1.0);
    vec3 baseColor = texColor.rgb * material.diffuseColor * VertexColor;
    
    // Obtener el color especular
    vec3 specColor = material.hasSpecularMap ? texture(material.specular, TexCoords).rgb : material.specularColor;

    // Recorte Alpha (Alpha Test)
    float alpha = texColor.a * material.opacity;
    if ((material.alphaTest || transparency == 1) && alpha < 0.1)
    {
        discard;
    }

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // Iluminación Direccional
    vec3 result = CalcDirLight(dirLight, norm, viewDir, baseColor, specColor);

    // Luces Puntuales
    for (int i = 0; i < NUMBER_OF_POINT_LIGHTS; i++)
    {
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir, baseColor, specColor);
    }

    // Luz Spot
    result += CalcSpotLight(spotLight, norm, FragPos, viewDir, baseColor, specColor);

    // Agregar Emisivo
    result += material.emissive;

    color = vec4(result, alpha);
}

// Cálculo de Luz Direccional
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 baseColor, vec3 specColor)
{
    vec3 lightDir = normalize(-light.direction);
    
    // Difusa
    float diff = max(dot(normal, lightDir), 0.0);
    
    // Especular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
    vec3 ambient = light.ambient * baseColor;
    vec3 diffuse = light.diffuse * diff * baseColor;
    vec3 specular = light.specular * spec * specColor;
    
    return (ambient + diffuse + specular);
}

// Cálculo de Luz Puntual
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 baseColor, vec3 specColor)
{
    vec3 lightDir = normalize(light.position - fragPos);
    
    // Difusa
    float diff = max(dot(normal, lightDir), 0.0);
    
    // Especular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
    // Atenuación
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    
    vec3 ambient = light.ambient * baseColor * attenuation;
    vec3 diffuse = light.diffuse * diff * baseColor * attenuation;
    vec3 specular = light.specular * spec * specColor * attenuation;
    
    return (ambient + diffuse + specular);
}

// Cálculo de Luz Spot
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 baseColor, vec3 specColor)
{
    vec3 lightDir = normalize(light.position - fragPos);
    
    // Difusa
    float diff = max(dot(normal, lightDir), 0.0);
    
    // Especular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
    // Atenuación e Intensidad
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    
    vec3 ambient = light.ambient * baseColor * attenuation * intensity;
    vec3 diffuse = light.diffuse * diff * baseColor * attenuation * intensity;
    vec3 specular = light.specular * spec * specColor * attenuation * intensity;
    
    return (ambient + diffuse + specular);
}