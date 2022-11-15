#version 450
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;
layout(location = 5) in int aMaterialNumber;

out VS_OUT {
    vec3 fragPosition;
    vec3 fragNormal;
    vec2 TexCoords;
    flat int materialNumber;

    mat3 tbnMat;
} vs_out;


uniform mat4 model;
uniform mat4 modelView;
uniform mat4 normal;

layout (std140, binding = 0) uniform Camera {
    uniform mat4 projection;
};

void main() {
    vs_out.fragPosition = (modelView * vec4(aPos, 1.0)).xyz;
    // multiplying by 1.000001 fixes interpolation problem that causes stripes in specular lighting on flat surfaces
    vs_out.fragNormal = (normal * vec4(aNormal * 1.000001, 0.0)).xyz;
    vs_out.TexCoords = aTexCoords;
    vs_out.materialNumber = aMaterialNumber;

    vec3 T = normalize(vec3(model * vec4(aTangent,   0.0)));
    vec3 B = normalize(vec3(model * vec4(aBitangent, 0.0)));
    vec3 N = normalize(vec3(model * vec4(aNormal,    0.0)));
    mat3 TBN = mat3(T, B, N);
    vs_out.tbnMat = TBN;

    gl_Position = projection * modelView * vec4(aPos, 1.0);
}
