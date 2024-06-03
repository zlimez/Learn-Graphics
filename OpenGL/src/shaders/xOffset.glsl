#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 color;

uniform float xOffset;

void main() {
    gl_Position = vec4(aPos.x + xOffset, aPos.yz, 1.0);
    color = aColor;
}