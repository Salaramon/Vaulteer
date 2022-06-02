#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out vec3 FragPosition;
out vec3 FragNormal;
out vec2 TexCoords;

uniform mat4 modelView;
uniform mat4 projection;
uniform mat4 normal;

void main() {   
    FragPosition = (modelView * vec4(aPos, 1.0)).xyz;
    FragNormal = (normal * vec4(aNormal, 0.0)).xyz;
    TexCoords = aTexCoords;

    gl_Position = projection * modelView * vec4(aPos, 1.0);
}