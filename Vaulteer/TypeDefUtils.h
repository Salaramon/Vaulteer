#pragma once

using byte = unsigned char;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;


// vec4 with padding - to use in uniformbuffer contents since GPU buffers do not properly support vec3
struct glsl_vec3 : public glm::vec4 {
	glsl_vec3() : glm::vec4() {}
	glsl_vec3(glm::vec3 in) : glm::vec4(in, 1.0) {}
};