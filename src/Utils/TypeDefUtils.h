#pragma once

using byte = unsigned char;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;


// vec4 with padding - to use in uniformbuffer contents since GPU buffers do not properly support vec3
struct glsl_vec3 : glm::vec4 {
	glsl_vec3() : glm::vec4() {}
	glsl_vec3(float in) : glm::vec4(in) {}
	glsl_vec3(float x, float y, float z) : glm::vec4(x,y,z, 1.0) {}
	glsl_vec3(glm::vec3 in) : glm::vec4(in, 1.0) {}
};