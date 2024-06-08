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
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
  
uniform mat4 view;
uniform Material material;
uniform Light light; 

void main() {
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(vec3(view * vec4(-light.direction, 0.0)));

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));  

    vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // vec3 specular = light.specular * spec * (vec3(1.0f) - vec3(texture(material.specular, TexCoords))); // inverted specular map
    vec3 specTex = vec3(texture(material.specular, TexCoords));
    vec3 specular = light.specular * spec * specTex;

    // vec3 emission = specTex == vec3(0.0) ? vec3(texture(material.emission, TexCoords)) : vec3(0.0);

    vec3 phong = ambient + diffuse + specular;
    FragColor = vec4(phong, 1.0);
}