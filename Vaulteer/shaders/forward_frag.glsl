#version 330 core
out vec4 FragColor;

in vec3 FragPosition;
in vec3 FragNormal;
in vec2 TexCoords;

uniform sampler2DArray textureLib;

uniform mat4 inverseViewMat;

uniform vec3 cameraPos;
uniform vec3 lightPos;

const int DIFFUSE_LAYER = 0;
const int SPECULAR_LAYER = 1;
const int NORMAL_LAYER = 2;

void main() {
    vec3 fragPosition = (inverseViewMat * vec4(FragPosition, 1.0)).xyz;

    vec3 lightColor = vec3(1.0);

    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
    
     // diffuse 
    vec3 norm = normalize(FragNormal);
    vec3 lightDir = normalize(lightPos - fragPosition);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // specular
    float specularStrength = texture(textureLib, vec3(TexCoords, SPECULAR_LAYER)).r;
    vec3 viewDir    = normalize(cameraPos - fragPosition);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), 64.0);
    vec3 specular = specularStrength * spec * lightColor; 

    vec3 result = (ambient + diffuse + specular) * texture(textureLib, vec3(TexCoords, DIFFUSE_LAYER)).rgb;
    FragColor = vec4(result, 1.0);
}