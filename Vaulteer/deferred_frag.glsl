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
    PointLight light;
    Attenuation att;
    vec3 position;
    float radius;
};

// constants
const int NUM_CASCADES = 3;
const int MAX_LIGHTS = 4;

// gbuffer data
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gColor;

// shadow maps
uniform sampler2D shadowMap_0;
uniform sampler2D shadowMap_1;
uniform sampler2D shadowMap_2;

uniform mat4 lightSpaceMatrices[NUM_CASCADES];
uniform float cascadeFarPlanes[NUM_CASCADES];

// shadow cube maps
uniform samplerCube shadowCubeMap_0;
uniform samplerCube shadowCubeMap_1;
uniform samplerCube shadowCubeMap_2;
uniform samplerCube shadowCubeMap_3;

uniform float cubeMapFarPlanes[MAX_LIGHTS];


// uniforms

uniform vec3 worldCameraPos;
uniform mat4 cameraViewMat;

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_LIGHTS];

uniform float materialSpecularIntensity;
uniform float materialShininess;

uniform vec3 fogColor;


vec3 sampleOffsetDirections[20] = vec3[]
(
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
);   

// functions

sampler2D getShadowMap(int cascadeIndex) {
    switch(cascadeIndex) {
    case 0: return shadowMap_0;
    case 1: return shadowMap_1;
    case 2: return shadowMap_2;
    default: return shadowMap_0;
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

vec4 calcLightInternal(BaseLight light, vec3 lightDirection, vec3 fragPosition, vec3 fragNormal, float shadow) {
    vec4 ambientColor = vec4(light.color, 1.0f) * light.ambientIntensity;
    vec4 diffuseColor = vec4(0.0);
    vec4 specularColor = vec4(0.0);

    float diffuseFactor = dot(fragNormal, -lightDirection);
    if (diffuseFactor > 0.0) {
        diffuseColor = vec4(light.color * diffuseFactor * light.diffuseIntensity, 1.0);

        float shininess = materialShininess * 4;
        float kEnergyConservation = (8.0 + shininess) / (8.0 * 3.14159265);
        vec3 viewDir = normalize(worldCameraPos - fragPosition);

        // blinn-phong shading impl
        vec3 halfwayDir = normalize(viewDir - lightDirection);
        float specularFactor = max(dot(fragNormal, halfwayDir), 0.0);
        
        // phong shading impl
        /*vec3 reflectDir = reflect(lightDirection, fragNormal);
        float specularFactor = max(dot(reflectDir, viewDir), 0.0);*/

        specularFactor = kEnergyConservation * pow(specularFactor, shininess);
        specularColor = vec4(light.color * min(materialSpecularIntensity * specularFactor, 1.0 - shadow), 1.0);
    }

    float adjustFactor = max(ambientColor.r + diffuseColor.r + specularColor.r, ambientColor.g + diffuseColor.g + specularColor.g);
          adjustFactor = max(ambientColor.b + diffuseColor.b + specularColor.b, adjustFactor);
          adjustFactor = max(adjustFactor, 1.0);

    return (ambientColor + diffuseColor + specularColor) * vec4(1.0 / adjustFactor);
}

vec4 calcPointLight(PointLight pointLight, vec3 fragPosition, vec3 fragNormal) {
    vec3 lightDirection = fragPosition - pointLight.position;
    float lightDistance = length(lightDirection);
    if (lightDistance > pointLight.radius)
        return vec4(0.0);

    float radiusCutoffFactor = min(1.0, (pointLight.radius - lightDistance) / (pointLight.radius / 10));

    lightDirection = normalize(lightDirection);

    float attenuation = (pointLight.att.aConstant
        + pointLight.att.aLinear * lightDistance
        + pointLight.att.aQuadratic * (lightDistance * lightDistance));
    attenuation = max(1.0, attenuation);

    return (calcLightInternal(pointLight.light, lightDirection, fragPosition, fragNormal, 0.0) / attenuation) * radiusCutoffFactor;
}

vec4 calcDirectionalLight(DirectionalLight dirLight, vec3 fragPosition, vec3 fragNormal, float shadow) {
    vec3 lightDirection = normalize(dirLight.direction);
    return calcLightInternal(dirLight.light, lightDirection, fragPosition, fragNormal, shadow);
}


float calcShadow(vec4 fragPosLightSpace, sampler2D shadowMap, int cascadeIndex) {
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5; 
    
    if(projCoords.z > 1.0)
        return 0.0;
    
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;

    float bias = 0.01;
    float shadow = 0.0;
    vec2 texelSize = (15.0 / cascadeFarPlanes[cascadeIndex]) / textureSize(shadowMap, 0);

    int sampling = 3;
    for(int x = -sampling; x <= sampling; ++x) {
        for(int y = -sampling; y <= sampling; ++y) {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
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


void main() {
    const float gamma = 2.0;

    if (texture(gPosition, TexCoords).w == 1.0) {
        FragColor = vec4(pow(fogColor, vec3(1.0 / gamma)), 1.0);
        return;
    } 
    vec3 fragPosition = texture(gPosition, TexCoords).xyz;
    vec3 fragNormal = texture(gNormal, TexCoords).xyz;
    vec3 diffuse = texture(gColor, TexCoords).rgb;
 
    float fragDepth = abs((cameraViewMat * vec4(fragPosition, 1.0)).z);

    // shadow calc
    
    int cascadeIndex = 0;
    for (int i = 0; i < NUM_CASCADES; i++) {
        if (fragDepth < cascadeFarPlanes[i]) {
            cascadeIndex = i;
            break;
        }
        cascadeIndex = NUM_CASCADES; 
    }

    vec4 fragPositionLightSpace = lightSpaceMatrices[cascadeIndex] * vec4(fragPosition, 1.0);
    float dirLightShadow = cascadeIndex > -1 ? 
            calcShadow(fragPositionLightSpace, getShadowMap(cascadeIndex), cascadeIndex) : 
            0.0; 

    float dirLightShadowFactor = (1.0 - dirLightShadow / 2);

    // light calc

    vec4 totalLight = calcDirectionalLight(directionalLight, fragPosition, fragNormal, dirLightShadow) * dirLightShadowFactor;


    for (int i = 0; i < MAX_LIGHTS; i++) {
        float pointLightShadow = calcShadowCube(fragPosition, getShadowCubeMap(i), i);
        totalLight += calcPointLight(pointLights[i], fragPosition, fragNormal) * (1.0 - pointLightShadow);
    }

    //float fogFactor = min(fragDepth / 20.0, 1.0);
    //vec4 fog = vec4((vec3(1.0) - vec3(1.0-fogColor.r, 1.0-fogColor.g, 1.0-fogColor.b) * fogFactor), 1.0);
    //vec4 fragColor = mix(totalLight, fog, fogFactor);

    vec4 fragColor = totalLight;
    FragColor = vec4(pow(fragColor.xyz, vec3(1.0 / gamma)), 1.0);
}
