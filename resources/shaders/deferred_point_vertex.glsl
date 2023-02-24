#version 450 core
layout(location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;

layout (std140, binding = 0) uniform Camera {
    uniform mat4 projection; //! uniform mat4 projection;
};


void main() {
    gl_Position = ((projection)) * view * model * vec4(aPos.xyz, 1.0);
}
