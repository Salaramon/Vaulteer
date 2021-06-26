#version 330 core
out vec4 FragColor;

in vec3 fragPosition;
in vec3 fragNormal;
in vec2 TexCoords;

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
};

struct SpotLight {
    PointLight base;
    vec3 direction;
    float cutoff;
};

// uniforms

uniform DirectionalLight directionalLight;
uniform PointLight pointLight;
uniform SpotLight spotLight;

uniform float materialSpecularIntensity;
uniform float materialShininess;

uniform vec3 worldCameraPos;

uniform sampler2D u_texture;

// functions

float rand(vec2 co) {
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

vec4 calcLightInternal(BaseLight light, vec3 lightDirection) {
    vec4 ambientColor = vec4(light.color, 1.0f) * light.ambientIntensity;
    vec4 diffuseColor = vec4(0.0);
    vec4 specularColor = vec4(0.0);

    float diffuseFactor = dot(normalize(fragNormal), -lightDirection);

    if (diffuseFactor > 0) {
        diffuseColor = vec4(light.color * diffuseFactor * light.diffuseIntensity, 1.0);

        float kEnergyConservation = (8.0 + materialShininess) / (8.0 * 3.14159265);

        vec3 viewDir = normalize(worldCameraPos - fragNormal);
        vec3 lightReflect = normalize(reflect(lightDirection, fragNormal));
        vec3 halfwayDir = normalize(lightDirection + viewDir);

        float specularFactor = max(dot(viewDir, lightReflect), 0.0);
        specularFactor = kEnergyConservation * pow(specularFactor, materialShininess);

        specularColor = vec4(light.color * materialSpecularIntensity * specularFactor, 1.0f);
    }

    return (ambientColor + diffuseColor + specularColor);
}

vec4 calcPointLight(PointLight pointLight) {
    vec3 lightDirection = fragPosition - pointLight.position;
    float lightDistance = length(lightDirection);
    lightDirection = normalize(lightDirection);

    float attenuation = (pointLight.att.aConstant
                       + pointLight.att.aLinear * lightDistance
                       + pointLight.att.aQuadratic * (lightDistance * lightDistance));

    return calcLightInternal(pointLight.light, lightDirection) / attenuation;
}

vec4 calcSpotLight(SpotLight spotLight) {
    vec3 lightToFragment = normalize(fragPosition - spotLight.base.position);
    float spotFactor = dot(lightToFragment, spotLight.direction);

    if (spotFactor > spotLight.cutoff) {
        vec4 color = calcPointLight(spotLight.base);
        return color * (1.0 - (1.0 - spotFactor) * 1.0 / (1.0 - spotLight.cutoff));
    }
    else {
        return vec4(0, 0, 0, 0);
    }
}

void main() {
    vec4 spotColor = calcSpotLight(spotLight);
    FragColor = spotColor + calcPointLight(pointLight);

    //FragColor = vec4(fragNormal, 1.0);
}