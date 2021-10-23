#version 330 core
out vec4 FragColor;

// uniforms

uniform vec3 lightColor;

// functions

void main() {
    FragColor = vec4(lightColor, 1.0);
}