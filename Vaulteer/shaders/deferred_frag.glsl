#version 450 core
in vec2 TexCoords;

out vec4 FragColor;

// light types

struct Attenuation {
    float aConstant;
    float aLinear;
    float aQuadratic;
};

struct BaseLight {
    vec3 color;
    float ambientIntensity;
    float diffuseIntensity;
};

struct DirectionalLight {
    BaseLight light;
    vec3 direction;
};

struct PointLight {
    BaseLight light;
    Attenuation att;
    vec3 position;
    float radius;
};

struct SpotLight {
    BaseLight light;
    Attenuation att;
    vec3 position;
    float radius;
    vec3 direction;
    float angle;
};

// constants
const int NUM_CASCADES = 1;
const int MAX_POINT_LIGHTS = 1;
const int MAX_SPOT_LIGHTS = 1;

const vec3 sampleOffsetDirections[20] = vec3[]
(
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
);   

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

uniform sampler2D shadowSpotMap_0;
uniform sampler2D shadowSpotMap_1;
uniform sampler2D shadowSpotMap_2;
uniform sampler2D shadowSpotMap_3;
uniform mat4 spotLightSpaceMatrices[MAX_SPOT_LIGHTS];

// shadow cube maps
uniform samplerCube shadowCubeMap_0;
uniform samplerCube shadowCubeMap_1;
uniform samplerCube shadowCubeMap_2;
uniform samplerCube shadowCubeMap_3;

uniform float cubeMapFarPlanes[MAX_POINT_LIGHTS];

// material data
struct Material {
    vec4 colorAmbient;
    vec4 colorDiffuse;
    vec4 colorSpecular;
    float shininess;
    float opacity;
};  

layout(shared, binding = 2) uniform MaterialData {
    uniform Material materialTable[6];
};

// uniforms

uniform vec3 worldCameraPos;
uniform mat4 cameraViewMat;

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];

uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

uniform float materialSpecularIntensity;
uniform float materialShininess;

uniform vec3 fogColor;


struct FragParams {
    vec3 fragPosition; 
    vec3 fragNormal; 
    int materialIndex;
    float specIntensity;
} params;

// functions


float rand(vec2 co) {
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

sampler2D getShadowMap(int cascadeIndex) {
    switch(cascadeIndex) {
    case 0: return shadowMap_0;
    case 1: return shadowMap_1;
    case 2: return shadowMap_2;
    default: return shadowMap_0;
    }
}

sampler2D getSpotShadowMap(int spotIndex) {
    switch(spotIndex) {
    case 0: return shadowSpotMap_0;
    case 1: return shadowSpotMap_1;
    case 2: return shadowSpotMap_2;
    case 3: return shadowSpotMap_3;
    default: return shadowSpotMap_0;
    }
}

samplerCube getShadowCubeMap(int pointLightIndex) {
    switch(pointLightIndex) {
    case 0: return shadowCubeMap_0;
    case 1: return shadowCubeMap_1;
    case 2: return shadowCubeMap_2;
    case 3: return shadowCubeMap_3;
    default: return shadowCubeMap_0;
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

float calcShadowCube(vec3 fragPos, samplerCube shadowCubeMap, int pointLightIndex) {
    // get vector between fragment position and light position
    vec3 fragToLight = fragPos - pointLights[pointLightIndex].position;
    // use the light to fragment vector to sample from the depth map
    float currentDepth = length(fragToLight);

    // now test for shadows
    float bias = 0.1;
    float shadow = 0.0;

    int samples = 20;
    float viewDistance = length(worldCameraPos - fragPos);
    float diskRadius = 0.05;
    for(int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(shadowCubeMap, fragToLight + sampleOffsetDirections[i] * diskRadius).r;
        closestDepth *= cubeMapFarPlanes[pointLightIndex];   // undo mapping [0;1]
        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(samples);  

    return shadow;
}

vec4 calcLightInternal(FragParams p, BaseLight light, vec3 lightDirection, float shadow) {
    Material mat = materialTable[p.materialIndex];

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

        specularFactor = p.specIntensity * kEnergyConservation * pow(specularFactor, mat.shininess);

        float shadowFactor = 1.0 - shadow;
        specularColor = vec4(light.color * specularFactor * shadowFactor, 1.0) * mat.colorSpecular;
    }

    float adjustFactor = max(ambientColor.r + diffuseColor.r + specularColor.r, ambientColor.g + diffuseColor.g + specularColor.g);
          adjustFactor = max(ambientColor.b + diffuseColor.b + specularColor.b, adjustFactor);
          adjustFactor = max(adjustFactor, 1.0);

    return (ambientColor + diffuseColor + specularColor) * adjustFactor;
}

vec4 calcPointLightInternal(FragParams p, BaseLight light, Attenuation att, vec3 lightDirection, float radius, float lightDistance) {
    const float pointLightAngleDropoff = 10.0; // determines light fade rate at edge

    if (lightDistance > radius)
        return vec4(0.0);

    float radiusCutoffFactor = min(1.0, (radius - lightDistance) / (radius / pointLightAngleDropoff));

    lightDirection = normalize(lightDirection);

    float attenuation = 1 / (att.aConstant
        + att.aLinear * lightDistance
        + att.aQuadratic * (lightDistance * lightDistance));

    return (calcLightInternal(p, light, lightDirection, 0.0) * attenuation) * radiusCutoffFactor;
}

vec4 calcPointLight(FragParams p, PointLight pointLight) {
    vec3 lightDirection = p.fragPosition - pointLight.position;
    float lightDistance = length(lightDirection);

    return calcPointLightInternal(p, pointLight.light, pointLight.att, lightDirection, pointLight.radius, lightDistance);
}

vec4 calcDirectionalLight(FragParams p, DirectionalLight dirLight, float shadow) {
    vec3 lightDirection = normalize(dirLight.direction);
    return calcLightInternal(p, dirLight.light, lightDirection, shadow);
}

vec4 calcSpotLight(FragParams p, SpotLight spotLight, int index) {
    const float spotLightAngleDropoff = 5.0; // determines light fade rate at edge
    const float spotLightAngleThreshold = 0.002; // hides early shadowmap dropoff

    vec3 lightDirection = p.fragPosition - spotLight.position;
    float theta = dot(normalize(lightDirection), spotLight.direction);
    float angle = cos(spotLight.angle / 2) + spotLightAngleThreshold;
    
    vec4 light = vec4(0.0);
    if(theta > angle) {
        float angleFactor = min(1.0, (theta - angle) / (theta / spotLightAngleDropoff));
        float lightDistance = length(lightDirection);
        light = calcPointLightInternal(p, spotLight.light, spotLight.att, lightDirection, spotLight.radius, lightDistance) * angleFactor;
        
        float spotLightShadow = calcShadow(p.fragPosition, p.fragNormal, spotLightSpaceMatrices[index], getSpotShadowMap(index), lightDirection, spotLight.radius);

        light *= (1.0 - spotLightShadow);
    }
    return light;
}


void main() {
    const float gamma = 1.0;

    // -------- reading from GBuffer --------

    vec3 positionSample = texture(gPosition, TexCoords).xyz;
    vec4 normalSample = texture(gNormal, TexCoords);
    vec4 colorSample = texture(gColor, TexCoords);

    // ------------- aggregation ------------

    params.fragPosition = vec4(inverse(cameraViewMat) * vec4(positionSample, 1.0)).xyz;
    
    params.fragNormal = normalSample.xyz;
    params.materialIndex = int(normalSample.w);

    vec3 diffuse = colorSample.rgb;
    params.specIntensity = colorSample.a;

    // --------------------------------------

    //FragColor = vec4(colorSample.a); //spec demo
    //FragColor = vec4(diffuse.rgb, 1.0); // diffuse demo

    //float material = texture(gMaterial, TexCoords).x;
 
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
    vec4 totalLight = calcDirectionalLight(params, directionalLight, dirLightShadow) * dirLightShadowFactor;
    
    for (int i = 0; i < MAX_SPOT_LIGHTS; i++) {
        totalLight += calcSpotLight(params, spotLights[i], i);
    }

    //FragColor = spotLightSpaceMatrices[0] * vec4(fragPosition, 1.0) / spotLights[0].radius;
    //return;

    for (int i = 0; i < MAX_POINT_LIGHTS; i++) {
        float pointLightShadow = 0.0;//calcShadowCube(fragPosition, getShadowCubeMap(i), i);
        totalLight += calcPointLight(params, pointLights[i]) * (1.0 - pointLightShadow);
    }

    //float fogFactor = min(fragDepth / 20.0, 1.0);
    //vec4 fog = vec4((vec3(1.0) - vec3(1.0-fogColor.r, 1.0-fogColor.g, 1.0-fogColor.b) * fogFactor), 1.0);
    //vec4 fragColor = mix(totalLight, fog, fogFactor);

    // kinda hdr impl
    totalLight = vec4(min(totalLight.x, 1.0), min(totalLight.y, 1.0), min(totalLight.z, 1.0), totalLight.w) + 
                 vec4(max(0.0,totalLight.x / 5.0 - 1.0), max(0.0,totalLight.y / 5.0 - 1.0), max(0.0,totalLight.z / 5.0 - 1.0), totalLight.w);

    vec4 fragColor = vec4(diffuse.x * totalLight.x, diffuse.y * totalLight.y, diffuse.z * totalLight.z, 1.0);
    FragColor = vec4(pow(fragColor.xyz, vec3(1.0 / gamma)), 1.0);
}
