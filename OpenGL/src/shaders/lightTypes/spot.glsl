#version 330 core
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

struct Light {
    bool atCam;
    vec3 position;
    vec3 direction;
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
uniform Light light; 

void main() {
    float dist = length((light.atCam ? vec3(0.0) : view * vec4(light.position, 1.0)) - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(vec3(light.atCam ? vec4(0.0) : view * vec4(light.position, 1.0)) - FragPos);

    float theta = dot(lightDir, (light.atCam ? vec3(0.0, 0.0, 1.0) : -light.direction));
    float epsilon = light.innerCone - light.outerCone;
    float intensity = clamp((theta - light.outerCone) / epsilon, 0.0, 1.0);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords)) * intensity;

    vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specTex = vec3(texture(material.specular, TexCoords));
    vec3 specular = light.specular * spec * specTex * intensity;

    // vec3 emission = specTex == vec3(0.0) ? vec3(texture(material.emission, TexCoords)) : vec3(0.0);

    vec3 phong = (ambient + diffuse + specular) * attenuation;
    FragColor = vec4(phong, 1.0);
}