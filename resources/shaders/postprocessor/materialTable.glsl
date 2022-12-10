// material data
struct Material {
    vec4 colorAmbient;
    vec4 colorDiffuse;
    vec4 colorSpecular;
    float shininess;
    float opacity;
    vec2 pad; // for alignment to 16
};

layout(shared, binding = 2) uniform MaterialData {
    uniform DFMaterial materialTable[%MAX_MATERIALS%];
};
