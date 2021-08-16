#version 450
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out vec3 fragPosition;
out vec3 fragNormal;
out vec2 TexCoords;
out vec4 fragPositionLightSpace;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 normal;

float rand(vec2 co) {
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

void main() {
    fragPosition = (model * vec4(aPos, 1.0)).xyz;
    fragNormal = (normal * vec4(aNormal, 0.0)).xyz;
    TexCoords = aTexCoords;

    gl_Position = projection * view * vec4(fragPosition, 1.0);
}
