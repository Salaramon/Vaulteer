#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D depthMap;

void main()
{   
    float gamma = 1.0;
    float depthValue = pow(texture(depthMap, TexCoords).r, gamma);
    FragColor = vec4(vec3(depthValue), 1.0);
}  
