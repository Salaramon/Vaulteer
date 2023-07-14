#version 450
layout(location = 0) in vec3 aPos;
layout(location = 5) in int aMaterialNumber;
layout(location = 6) in mat4 instance;

out int materialNumber;
out int instanceID;

layout (std140, binding = 0) uniform Camera {
    uniform mat4 view;
    uniform mat4 projection;
    uniform mat4 screenProjection;
};

void main() {
    materialNumber = aMaterialNumber;
    instanceID = gl_InstanceID;
    gl_Position = projection * view * instance * vec4(aPos, 1.0);
}