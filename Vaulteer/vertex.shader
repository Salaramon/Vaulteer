#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

void main()
{
    TexCoords = aTexCoords;    
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}

#version 330 core

layout(location = 1) in vec3 input0;
layout(location = 2) in vec3 input1;
layout(location = 3) in vec2 input2;

out vec2 output0;

uniform mat4 uniform0;
uniform mat4 uniform1;
uniform mat4 uniform2;

void main()
{
	output0 = input2;
	gl_Position = uniform0 * uniform1 * uniform2 * vec3(input0, 1.000000);
}