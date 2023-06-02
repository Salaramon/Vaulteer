#version 450 core
layout(location = 0) in vec3 aPos;

flat out int instanceID;

void main() {
    instanceID = gl_InstanceID;
    gl_Position = vec4(aPos.xyz, 1.0);
}
