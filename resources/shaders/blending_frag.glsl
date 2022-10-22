#version 450 core

// multiple render targets - renders to AlphaBuffer
layout(location = 0) out vec4 Accum;
layout(location = 1) out float Reveal;

in VS_OUT {
    vec3 fragPosition;
    vec3 fragNormal;
    vec2 TexCoords;
    flat int materialNumber;
} fs_in;

uniform sampler2DArray textureLib;

uniform mat4 inverseViewMat;

uniform vec3 cameraPos;
uniform vec3 lightPos;

struct BFModelUnitData {
    int xDelta; 
    int yDelta;
    int wDelta;
    int hDelta;
    int layerDelta;
};

layout(shared, binding = 1) uniform ModelUnitTables {
    uniform BFModelUnitData unitTable[384];
};

const int diffuse_unit_index = 0;
const int specular_unit_index = 1;
const int normals_unit_index = 2;

// material data
struct BFMaterial {
    vec4 colorAmbient;
    vec4 colorDiffuse;
    vec4 colorSpecular;
    float shininess;
    float opacity;
    vec2 pad; // for alignment to 16
};

layout(shared, binding = 2) uniform MaterialData {
    uniform BFMaterial materialTable[128];
};

vec3 getTexUnitCoords(vec3 texCoords, int materialNumber, int unitIndex) {
    return vec3(texCoords.x + float(unitTable[materialNumber * 3 + unitIndex].xDelta) / textureSize(textureLib,0).x, 
                texCoords.y + float(unitTable[materialNumber * 3 + unitIndex].yDelta) / textureSize(textureLib,0).y,
                texCoords.z + unitTable[materialNumber * 3 + unitIndex].layerDelta); 
}

void main() {
    vec3 fragPosition = (inverseViewMat * vec4(fs_in.fragPosition, 1.0)).xyz;
    //vec3 fragNormal = fs_in.fragNormal;

    BFMaterial mat = materialTable[fs_in.materialNumber];

    vec3 lightColor = vec3(1.0);

    vec3 ambient = vec3(0.1);
    
     // diffuse
    vec3 norm = normalize(fs_in.fragNormal);
    vec3 lightDir = normalize(lightPos - fragPosition);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // specular
    vec2 specularDelta = vec2(unitTable[fs_in.materialNumber * 3 + specular_unit_index].xDelta, unitTable[fs_in.materialNumber * 3 + specular_unit_index].yDelta);
    float specularStrength;
    if (specularDelta != vec2(0.0)){ // specular map exists?
        vec3 specularCoords = getTexUnitCoords(vec3(fs_in.TexCoords, 0), fs_in.materialNumber, specular_unit_index);
        specularStrength = texture(textureLib, specularCoords).r;
    }
    else 
        specularStrength = 0.8;
    
    vec3 viewDir    = normalize(cameraPos - fragPosition);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), 64.0);
    vec3 specular = specularStrength * spec * lightColor; 
    
    vec3 diffuseCoords = vec3(fs_in.TexCoords, 0);
    //vec4 color = vec4((ambient + diffuse + specular) * texture(textureLib, diffuseCoords).rgb, mat.opacity);
    vec4 color = vec4((ambient + diffuse + specular) * texture(textureLib, diffuseCoords).rgb, 0.4);
    
    
    // weight function
    float weight = clamp(pow(min(1.0, color.a * 10.0) + 0.01, 3.0) * 1e8 * 
                         pow(1.0 - gl_FragCoord.z * 0.9, 3.0), 1e-2, 3e3);

    // store pixel color accumulation and pixel revealage threshold
    Accum = vec4(color.rgb * color.a, color.a) * weight;
    Reveal = color.a;
}