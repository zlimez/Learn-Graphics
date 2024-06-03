#version 330 core
out vec4 FragColor;

in vec3 color;
in vec2 TexCoords;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float visibilityRatio;

void main() {
    FragColor = mix(texture(texture1, vec2(-TexCoords.x, TexCoords.y)), texture(texture2, vec2(-TexCoords.x, TexCoords.y)), visibilityRatio);
}