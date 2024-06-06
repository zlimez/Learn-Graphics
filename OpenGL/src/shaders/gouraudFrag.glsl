#version 330 core
in vec3 PhongLight;

out vec4 FragColor;

uniform vec3 objectColor;

void main() {
    FragColor = vec4(PhongLight * objectColor, 1.0);
}