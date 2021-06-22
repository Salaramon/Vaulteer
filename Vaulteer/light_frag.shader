#version 330 core
out vec4 FragColor;

in vec3 worldPos;
in vec3 modelNormal;
in vec2 TexCoords;

struct DirectionalLight
{
    float ambientIntensity;
    vec3 color;
    float diffuseIntensity;
    vec3 direction;
};

uniform sampler2D u_texture;
uniform DirectionalLight directionalLight;

uniform vec3 worldCameraPos;

uniform float materialSpecularIntensity;
uniform float specularPower;


void main() {
    vec4 ambientColor = vec4(directionalLight.color, 1.0f) * directionalLight.ambientIntensity;

    vec4 specularColor = vec4(0, 0, 0, 0);
    vec4 diffuseColor = vec4(0.0);

    float diffuseFactor = dot(normalize(modelNormal), -directionalLight.direction);

    if (diffuseFactor > 0) {
        diffuseColor = vec4(directionalLight.color * diffuseFactor * directionalLight.diffuseIntensity, 1.0);

        vec3 vertexToEye = normalize(worldCameraPos - worldPos);
        vec3 lightReflect = normalize(reflect(directionalLight.direction, modelNormal));
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

    FragColor = diffuseColor + specularColor;
}