#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform float ambientStr;
uniform float specStr;
uniform float shininess;

out vec3 PhongLight;

void main()
{
    vec3 vtxPos = vec3(view * model * vec4(aPos, 1.0));
    gl_Position = projection * vec4(vtxPos, 1.0);
    vec3 ambient = ambientStr * lightColor;

    vec3 norm = normalize(mat3(transpose(inverse(view * model))) * aNormal);
    vec3 lightDir = normalize(vec3(view * vec4(lightPos, 1.0)) - vtxPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 viewDir = normalize(-vtxPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = spec * lightColor;

    PhongLight = ambient + diff + specular;
} 