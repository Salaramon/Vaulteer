#version 450 core
out vec4 FragColor;

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

/*
struct FFModelUnitData {
    int xDelta; 
    int yDelta;
    int wDelta;
    int hDelta;
    int layerDelta;
};

layout(shared, binding = 1) uniform ModelUnitTables {
    uniform FFModelUnitData unitTable[384];
};
*/

const int diffuse_unit_index = 0;
const int specular_unit_index = 1;
const int normals_unit_index = 2;

/*
// material data
struct FFMaterial {
    vec4 colorAmbient;
    vec4 colorDiffuse;
    vec4 colorSpecular;
    float shininess;
    float opacity;
    vec2 pad;
};  

layout(shared, binding = 2) uniform MaterialData {
    uniform FFMaterial materialTable[128];
};
*/

/*
vec3 getTexUnitCoords(vec3 texCoords, int materialNumber, int unitIndex) {
    return vec3(texCoords.x + float(unitTable[materialNumber * 3 + unitIndex].xDelta) / textureSize(textureLib,0).x, 
                texCoords.y + float(unitTable[materialNumber * 3 + unitIndex].yDelta) / textureSize(textureLib,0).y,
                texCoords.z + unitTable[materialNumber * 3 + unitIndex].layerDelta); 
}
*/

void main() {
    FragColor = vec4(1.0);
    return;

    vec3 fragPosition = (inverseViewMat * vec4(fs_in.fragPosition, 1.0)).xyz;
    //vec3 fragNormal = fs_in.fragNormal;

    vec3 lightColor = vec3(1.0);
    vec3 ambient = vec3(0.1);
    
     // diffuse
    vec3 norm = normalize(fs_in.fragNormal);
    vec3 lightDir = normalize(lightPos - fragPosition);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // specular
    float specularStrength = 0.8;
    
    vec3 viewDir    = normalize(cameraPos - fragPosition);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), 64.0);
    vec3 specular = specularStrength * spec * lightColor; 
    
    vec3 diffuseCoords = vec3(fs_in.TexCoords, 0);
    vec3 result = (ambient + diffuse + specular) * texture(textureLib, diffuseCoords).rgb;

    FragColor = vec4(result, 1.0);
}