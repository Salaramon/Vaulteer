#version 450 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;

out vec3 fragPos;
out vec2 texCoords;

layout (std140, binding = 0) uniform Camera {
    uniform mat4 view;
    uniform mat4 projection;
    uniform mat4 screenProjection;
};

void main() {
    texCoords = aTexCoords;
    gl_Position = screenProjection * vec4(aPos.xyz, 1.0);
}