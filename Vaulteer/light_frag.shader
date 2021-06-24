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

// uniforms

uniform sampler2D u_texture;
uniform DirectionalLight directionalLight;
uniform PointLight pointLight;

uniform vec3 worldCameraPos;

uniform float materialSpecularIntensity;
uniform float specularPower;

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

        vec3 vertexToEye = normalize(worldCameraPos - fragNormal);
        vec3 lightReflect = normalize(reflect(lightDirection, fragNormal));
        float specularFactor = dot(vertexToEye, lightReflect);
        if (specularFactor > 0) {
            specularFactor = pow(specularFactor, specularPower);
            specularColor = vec4(light.color * materialSpecularIntensity * specularFactor, 1.0f);
        }
    }

    return (ambientColor + diffuseColor + specularColor);
}

vec4 calcPointLight() {
    vec3 lightDirection = fragPosition - pointLight.position;
    float lightDistance = length(lightDirection);
    lightDirection = normalize(lightDirection);

    float attenuation = (pointLight.att.aConstant
                       + pointLight.att.aLinear * lightDistance
                       + pointLight.att.aQuadratic * (lightDistance * lightDistance));

    return calcLightInternal(pointLight.light, lightDirection) / attenuation;
}

void main() {
    FragColor = calcPointLight();
    //FragColor = vec4(fragNormal, 1.0);
}