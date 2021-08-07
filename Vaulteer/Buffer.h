#pragma once

#include <glad/glad.h>

class Buffer
{
public:
	Buffer() {}
	Buffer(Buffer&& other) noexcept : buffer(other.buffer) {
		other.buffer = 0;
	}
	~Buffer() {
		cleanup();
	}
protected:
	void initialize();
	void cleanup();

	GLuint buffer;
};

