#version 330 core
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform mat4 view;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform float ambientStr;
uniform float specStr;
uniform float shininess;

void main() {
    vec3 ambient = ambientStr * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(vec3(view * vec4(lightPos, 1.0)) - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = spec * lightColor;

    vec3 phong = (ambient + diff + specular) * objectColor;
    FragColor = vec4(phong, 1.0);
}