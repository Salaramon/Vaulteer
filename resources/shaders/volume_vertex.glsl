#version 450
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

out vec3 vpos;

layout (std140, binding = 0) uniform Camera {
    uniform mat4 view;
    uniform mat4 projection;
    uniform mat4 screenProjection;
};

uniform mat4 model;

void main() {
	vpos = (view * model * vec4(aPos, 1.0)).xyz;
}