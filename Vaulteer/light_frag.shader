#version 330 core
out vec4 FragColor;

in vec3 fragPos;
in vec2 TexCoords;

// light types

struct Attenuation {
    float aConstant;
    float aLinear;
    float aExp;
};

struct DirectionalLight {
    vec3 color;
    float ambientIntensity;
    float diffuseIntensity;
    vec3 direction;
};

struct PointLight {
    vec3 color;
    float ambientIntensity;
    float diffuseIntensity;
    vec3 position;
    Attenuation att;
};

// uniforms

uniform sampler2D u_texture;
uniform DirectionalLight directionalLight;

uniform vec3 worldCameraPos;

uniform float materialSpecularIntensity;
uniform float specularPower;

// functions

float rand(vec2 co) {
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

void main() {
    vec4 ambientColor = vec4(directionalLight.color, 1.0f) * directionalLight.ambientIntensity;
    vec4 diffuseColor = vec4(0.0);
    vec4 specularColor = vec4(0.0);

    float diffuseFactor = dot(normalize(fragPos), -directionalLight.direction);

    if (diffuseFactor > 0) {
        diffuseColor = vec4(directionalLight.color * diffuseFactor * directionalLight.diffuseIntensity, 1.0);

        vec3 vertexToEye = normalize(worldCameraPos - fragPos);
        vec3 lightReflect = normalize(reflect(directionalLight.direction, fragPos));
        float specularFactor = dot(vertexToEye, lightReflect);
        if (specularFactor > 0) {
            specularFactor = pow(specularFactor, specularPower);
            specularColor = vec4(directionalLight.color * materialSpecularIntensity * specularFactor, 1.0f);
        }
    }

    float ring = specularColor.x - 1.0;
    if (specularColor.x > 1.0) {
        specularColor -= ring * 2;
    }

    float rand = rand(vec2(fragPos));
    FragColor = (ambientColor + diffuseColor + specularColor);
}