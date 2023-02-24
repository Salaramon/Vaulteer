#version 450

// multiple render targets - renders to GBuffer
layout(location = 0) out vec3 gPosition;
layout(location = 1) out vec4 gNormal;
layout(location = 2) out vec4 gColor;


in VS_OUT {
    vec3 fragPosition;
    vec3 fragNormal;
    vec2 TexCoords;
    flat int materialNumber;

    mat3 tbnMat;
} fs_in;


struct ModelUnitData {
    int xDelta;
    int yDelta;
    int wDelta;
    int hDelta;
    int layerDelta;
};

layout(shared, binding = 1) uniform ModelUnitTables {
    uniform ModelUnitData unitTable[384];
};


uniform sampler2DArray textureLib;

const int diffuse_unit_index = 0;
const int specular_unit_index = 1;
const int normals_unit_index = 2;

vec3 getTexUnitCoords(vec3 texCoords, int materialNumber, int unitIndex) {
    return vec3(texCoords.x + float(unitTable[materialNumber * 3 + unitIndex].xDelta) / textureSize(textureLib,0).x, 
                texCoords.y + float(unitTable[materialNumber * 3 + unitIndex].yDelta) / textureSize(textureLib,0).y,
                texCoords.z + unitTable[materialNumber * 3 + unitIndex].layerDelta); 
}

void main() {
    vec3 diffuseCoords = vec3(fs_in.TexCoords, 0);

    vec2 normalsDelta = vec2(unitTable[fs_in.materialNumber * 3 + normals_unit_index].xDelta, unitTable[fs_in.materialNumber * 3 + normals_unit_index].yDelta);
    vec3 normal;
    if (normalsDelta != vec2(0.0)) { // normal map exists?
        vec3 normalCoords = getTexUnitCoords(vec3(fs_in.TexCoords, 0), fs_in.materialNumber, normals_unit_index);
        normal = normalize(fs_in.tbnMat * vec3(texture(textureLib, normalCoords).rgb * 2.0 - 1.0));
    }
    else
        normal = fs_in.fragNormal;

    vec2 specularDelta = vec2(unitTable[fs_in.materialNumber * 3 + specular_unit_index].xDelta, unitTable[fs_in.materialNumber * 3 + specular_unit_index].yDelta);
    float specular;
    if (specularDelta != vec2(0.0)){ // specular map exists?
        vec3 specularCoords = getTexUnitCoords(vec3(fs_in.TexCoords, 0), fs_in.materialNumber, specular_unit_index);
        specular = texture(textureLib, specularCoords).r;
    }
    else 
        specular = 0.8;
        
    gPosition = fs_in.fragPosition;
    gNormal = vec4(normal, float(fs_in.materialNumber));
    gColor = vec4(texture(textureLib, diffuseCoords).rgb, specular);
}
