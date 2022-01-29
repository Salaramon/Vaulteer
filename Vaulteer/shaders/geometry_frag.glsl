#version 450

// multiple render targets, gbuffer
layout(location = 0) out vec3 gPosition;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec4 gColor;

in vec3 fragPosition;
in vec3 fragNormal;
in vec2 TexCoords;
//in uint renderFlags;

uniform sampler2DArray textureLib;

void main()
{
    gPosition = fragPosition;
    gNormal = normalize(fragNormal);
    gColor = vec4(texture(textureLib, vec3(TexCoords, 0)).rgb, texture(textureLib, vec3(TexCoords, 1)).r);
}
