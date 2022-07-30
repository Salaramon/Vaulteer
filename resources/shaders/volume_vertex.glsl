#version 430
layout (location=0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;

out vec3 vertexPosition;

uniform mat4 modelView;

void main()
{ 
	vertexPosition = (modelView * vec4(aPosition,1.0)).xyz;
}