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



void main()
{
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Diffuse = texture(gColor, TexCoords).rgb;

    FragColor = vec4(
        texture(gPosition, TexCoords).rgb, 1.0);
}