#version 450 core
out vec4 FragColor;

// light types

struct DDFBaseLight {
    vec3 color;
    float ambientIntensity;
    float diffuseIntensity;
};


// constants
const int NUM_CASCADES = 1;

//##scenevars(lights)

const vec3 sampleOffsetDirections[20] = vec3[]
(
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1),
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
);

//#scenevar
const int NUM_MATERIALS = 10;

// gbuffer data
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gColor;

// shadow cascade maps
uniform sampler2D shadowMap_0;
uniform sampler2D shadowMap_1;
uniform sampler2D shadowMap_2;

uniform mat4 lightSpaceMatrices[NUM_CASCADES];
uniform float cascadeFarPlanes[NUM_CASCADES];


// ------------- replace with material macro
struct DDFMaterial {
    vec4 colorAmbient;
    vec4 colorDiffuse;
    vec4 colorSpecular;
    float shininess;
    float opacity;
    vec2 pad; // for alignment to 16
};

layout(shared, binding = 2) uniform MaterialData {
    uniform DDFMaterial materialTable[128];
};
// ------------- replace with material macro end

// ------------- replace with dirlight macro
struct DirectionalLight {
    DDFBaseLight light;
    vec3 direction;
};

struct UBDirectionalLight {
    vec3 color;
    float ambientIntensity;
    float diffuseIntensity;
    vec3 direction;
};

layout(shared, binding = 3) uniform DirectionalLightData {
    uniform UBDirectionalLight directionalLight;
};

DirectionalLight lightFromUBDirectionalLight(UBDirectionalLight ubl) {
    DirectionalLight l;
    l.light.color = ubl.color;
    l.light.ambientIntensity = ubl.ambientIntensity;
    l.light.diffuseIntensity = ubl.diffuseIntensity;
    l.direction = ubl.direction;
    return l;
}
// ------------- replace with dirlight macro end

// uniforms

uniform vec3 worldCameraPos;
uniform mat4 cameraViewMat;

uniform vec3 fogColor;


struct DDFFragParams {
    vec3 fragPosition;
    vec3 fragNormal;
    int materialIndex;
    float specIntensity;
} params;

// functions

sampler2D getShadowMap(int cascadeIndex) {
    switch(cascadeIndex) {
    case 0: return shadowMap_0;
    case 1: return shadowMap_1;
    case 2: return shadowMap_2;
    default: return shadowMap_0;
    }
}

float calcShadow(vec3 fragPos, vec3 fragNormal, mat4 lightSpaceMat, sampler2D shadowMap, vec3 lightDirection, float farPlane) {
    vec4 fragPosLightSpace = lightSpaceMat * vec4(fragPos, 1.0);

    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    if(projCoords.z > 1.0)
        return 0.0;
    
    float currentDepth = projCoords.z;
    
    float bias = max(0.02 * (1.0 - dot(fragNormal, lightDirection)), 0.005) * (currentDepth);

    float shadow = 0.0;
    vec2 texelSize = (15.0 / farPlane) / textureSize(shadowMap, 0);

    int sampling = 2;
    for(int x = -sampling; x <= sampling; ++x) {
        for(int y = -sampling; y <= sampling; ++y) {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += ((currentDepth - bias) > pcfDepth ? 1.0 : 0.0);        
        }    
    }
    shadow /= pow(sampling * 2 + 1, 2.0);
    return shadow;
}

vec4 calcLightInternal(DDFFragParams p, DDFBaseLight light, vec3 lightDirection, float shadow) {
    DDFMaterial mat = materialTable[p.materialIndex];

    vec4 ambientColor = vec4(light.color, 1.0f) * light.ambientIntensity;
    vec4 diffuseColor = vec4(0.0);
    vec4 specularColor = vec4(0.0);

    float diffuseFactor = dot(p.fragNormal, -lightDirection);
    if (diffuseFactor > 0.0) {
        diffuseColor = vec4(light.color * diffuseFactor * light.diffuseIntensity, 1.0);

        float kEnergyConservation = (8.0 + mat.shininess) / (8.0 * 3.14159265);
        vec3 viewDir = normalize(worldCameraPos - p.fragPosition);

        // blinn-phong shading impl
        vec3 halfwayDir = normalize(viewDir - lightDirection);
        float specularFactor = max(dot(p.fragNormal, halfwayDir), 0.0);

        // phong shading impl
        /*vec3 reflectDir = reflect(lightDirection, fragNormal);
        float specularFactor = max(dot(reflectDir, viewDir), 0.0);*/

        specularFactor = p.specIntensity * kEnergyConservation * pow(specularFactor, mat.shininess / 5.0);

        float shadowFactor = 1.0 - shadow;
        specularColor = vec4(light.color * specularFactor * shadowFactor, 1.0);

        //specularColor = vec4(specularFactor, 0.0, 0.0, 1.0); // specfactor is weird when material shininess is 0
    }

    float adjustFactor = max(ambientColor.r + diffuseColor.r + specularColor.r, ambientColor.g + diffuseColor.g + specularColor.g);
          adjustFactor = max(ambientColor.b + diffuseColor.b + specularColor.b, adjustFactor);
          adjustFactor = max(adjustFactor, 1.0);

    return (ambientColor + diffuseColor + specularColor) * adjustFactor;
}

vec4 calcDirectionalLight(DDFFragParams p, DirectionalLight dirLight, float shadow) {
    vec3 lightDirection = normalize(dirLight.direction);
    return calcLightInternal(p, dirLight.light, lightDirection, shadow);
}


void main() {
    const float gamma = 1.0;

    // -------- reading from GBuffer --------
    
    ivec2 coords = ivec2(gl_FragCoord.xy);
    vec3 positionSample = texelFetch(gPosition, coords, 0).xyz;
    vec4 normalSample = texelFetch(gNormal, coords, 0);
    vec4 colorSample = texelFetch(gColor, coords, 0);

    // ------------- aggregation ------------

    params.fragPosition = vec4(inverse(cameraViewMat) * vec4(positionSample, 1.0)).xyz;
    
    params.fragNormal = normalSample.xyz;
    params.materialIndex = int(normalSample.w);

    vec3 diffuse = colorSample.rgb;
    params.specIntensity = colorSample.a;

    // --------------------------------------
 
    float fragDepth = abs((cameraViewMat * vec4(params.fragPosition, 1.0)).z);

    // shadow calc
    
    /*int cascadeIndex = 0;
    for (int i = 0; i < NUM_CASCADES; i++) {
        if (fragDepth < cascadeFarPlanes[i]) {
            cascadeIndex = i;
            break;
        }
        cascadeIndex = NUM_CASCADES; 
    }

    float dirLightShadow = cascadeIndex > -1 ? 
            calcShadow(fragPosition, fragNormal, lightSpaceMatrices[cascadeIndex], getShadowMap(cascadeIndex), directionalLight.direction, cascadeFarPlanes[cascadeIndex]) :
            0.0; 

    float dirLightShadowFactor = (1.0 - dirLightShadow / 1.5);*/

    float dirLightShadow = 0.0;
    float dirLightShadowFactor = 1.0;

    // light calc
    vec4 totalLight = calcDirectionalLight(params, lightFromUBDirectionalLight(directionalLight), dirLightShadow) * dirLightShadowFactor;
    //FragColor = vec4(diffuse, 1.0);
    //return;

    
    // kinda hdr impl
    
    totalLight = vec4(min(totalLight.x, 1.0), min(totalLight.y, 1.0), min(totalLight.z, 1.0), totalLight.w) + 
                 vec4(max(0.0,totalLight.x / 5.0 - 1.0), max(0.0,totalLight.y / 5.0 - 1.0), max(0.0,totalLight.z / 5.0 - 1.0), totalLight.w);
                 
    vec4 fragColor = vec4(diffuse.x * totalLight.x, diffuse.y * totalLight.y, diffuse.z * totalLight.z, 1.0);
    FragColor = vec4(pow(fragColor.xyz, vec3(1.0 / gamma)), 1.0);
}
