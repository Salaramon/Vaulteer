#version 330 core
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

struct PointLight {
    BaseLight light;
    Attenuation att;
    vec3 position;
};

// gbuffer data

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gColor;

// uniforms

uniform vec3 worldCameraPos;

uniform PointLight pointLight;

uniform float materialSpecularIntensity;
uniform float materialShininess;

// functions

vec4 calcLightInternal(BaseLight light, vec3 lightDirection, vec3 fragPosition, vec3 fragNormal) {
    vec4 ambientColor = vec4(light.color, 1.0f) * light.ambientIntensity;
    vec4 diffuseColor = vec4(0.0);
    vec4 specularColor = vec4(0.0);

    float diffuseFactor = dot(normalize(fragNormal), -lightDirection);

    if (diffuseFactor > 0) {
        diffuseColor = vec4(light.color * diffuseFactor * light.diffuseIntensity, 1.0);

        float shininess = materialShininess * 4;
        float kEnergyConservation = (8.0 + shininess) / (8.0 * 3.14159265);
        vec3 viewDir = normalize(worldCameraPos - fragPosition);

        // blinn-phong shading impl
        vec3 halfwayDir = normalize(viewDir - lightDirection);
        float specularFactor = max(dot(fragNormal, halfwayDir), 0.0);

        specularFactor = kEnergyConservation * pow(specularFactor, shininess);
        specularColor = vec4(light.color * materialSpecularIntensity * specularFactor, 1.0);
    }

    return (ambientColor + diffuseColor + specularColor);
}

vec4 calcPointLight(PointLight pointLight, vec3 fragPosition, vec3 fragNormal) {
    vec3 lightDirection = fragPosition - pointLight.position;
    float lightDistance = length(lightDirection);
    lightDirection = normalize(lightDirection);

    float attenuation = (pointLight.att.aConstant
        + pointLight.att.aLinear * lightDistance
        + pointLight.att.aQuadratic * (lightDistance * lightDistance));

    return calcLightInternal(pointLight.light, lightDirection, fragPosition, fragNormal) / attenuation;
}


void main()
{
    vec3 fragPosition = texture(gPosition, TexCoords).xyz;
    vec3 fragNormal = texture(gNormal, TexCoords).xyz;
    vec3 diffuse = texture(gColor, TexCoords).xyz;

    FragColor = calcPointLight(pointLight, fragPosition, fragNormal);
}