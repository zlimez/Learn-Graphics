#version 330 core
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;

struct Material {
    sampler2D texture_diffuse0;
    sampler2D texture_specular0;
    sampler2D emission;
    float shininess;
};

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct FlashLight {
    float innerCone;
    float outerCone;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

uniform mat4 view;
uniform Material material;
uniform DirLight dirLight;
#define NR_POINT_LIGHTS 4  
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform FlashLight flashLight;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcFlashLight(FlashLight light, vec3 norm, vec3 fragPos, vec3 viewDir);

void main() {
    vec3 norm = normalize(Normal);  
    vec3 viewDir = normalize(-FragPos);
    vec3 finalColor = CalcDirLight(dirLight, norm, viewDir);
    for (int i = 0; i < NR_POINT_LIGHTS; i++)
        finalColor += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    finalColor += CalcFlashLight(flashLight, norm, FragPos, viewDir);
    FragColor = vec4(finalColor, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 norm, vec3 viewDir) {
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse0, TexCoords));

    vec3 lightDir = normalize(vec3(view * vec4(-light.direction, 0.0)));

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse0, TexCoords));  

    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specTex = vec3(texture(material.texture_specular0, TexCoords));
    vec3 specular = light.specular * spec * specTex;

    vec3 phong = ambient + diffuse + specular;
    return phong;
}

vec3 CalcPointLight(PointLight light, vec3 norm, vec3 fragPos, vec3 viewDir) {
    vec3 fragToLight = vec3(view * vec4(light.position, 1.0)) - FragPos;
    float dist = length(fragToLight);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse0, TexCoords));

    vec3 lightDir = normalize(fragToLight);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse0, TexCoords));  

    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specTex = vec3(texture(material.texture_specular0, TexCoords));
    vec3 specular = light.specular * spec * specTex;

    vec3 phong = (ambient + diffuse + specular) * attenuation;
    return phong;
}

vec3 CalcFlashLight(FlashLight light, vec3 norm, vec3 fragPos, vec3 viewDir) {
    float dist = length(-fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse0, TexCoords));
    vec3 lightDir = viewDir;

    float theta = dot(lightDir, vec3(0.0, 0.0, 1.0));
    float epsilon = light.innerCone - light.outerCone;
    float intensity = clamp((theta - light.outerCone) / epsilon, 0.0, 1.0);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse0, TexCoords)) * intensity;

    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specTex = vec3(texture(material.texture_specular0, TexCoords));
    vec3 specular = light.specular * spec * specTex * intensity;

    vec3 phong = (ambient + diffuse + specular) * attenuation;
    return phong;
}
