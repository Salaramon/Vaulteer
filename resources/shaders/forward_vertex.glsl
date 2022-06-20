#version 450 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;
layout(location = 5) in int aModelNumber;

out VS_OUT {
    vec3 fragPosition;
    vec3 fragNormal;
    vec2 TexCoords;
    flat int modelNumber;
} vs_out;

uniform mat4 modelView;
uniform mat4 normal;

layout (std140, binding = 0) uniform Camera {
    uniform mat4 projection;
};

void main() {   
    vs_out.fragPosition = (modelView * vec4(aPos, 1.0)).xyz;
    vs_out.fragNormal = (normal * vec4(aNormal, 0.0)).xyz;
    vs_out.TexCoords = aTexCoords;
    vs_out.modelNumber = aModelNumber;
       
    gl_Position = projection * modelView * vec4(aPos, 1.0);
}