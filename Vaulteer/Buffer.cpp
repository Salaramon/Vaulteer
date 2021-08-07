#include "Buffer.h"

void Buffer::initialize() {
	glGenBuffers(1, &buffer);

}

void Buffer::cleanup()
{
	glDeleteBuffers(1, &buffer);
}