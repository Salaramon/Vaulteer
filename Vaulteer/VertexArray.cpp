#include "VertexArray.h"

VertexArray::VertexArray() {
	initialize();
	debug("VertexArray created VAO: " + std::to_string(VAO) + "\n", "DEFAULT_CONSTRUCTOR");
}

VertexArray::~VertexArray() {
	cleanup();
	debug("VertexArray destroyed. VAO: " + std::to_string(VAO) + "\n", "DECONSTRUCTOR");
}

void VertexArray::bind() const {
	glBindVertexArray(VAO);
}

void VertexArray::unbind() const {
	glBindVertexArray(0);
}

GLuint VertexArray::bindIndex() const {
	return VAO;
}

void VertexArray::initialize() {
	glCreateVertexArrays(1, &VAO);
}

void VertexArray::cleanup() {
	glDeleteVertexArrays(1, &VAO);
}

void VertexArray::setUpAttributes(LocationVector locInfo, LocationVector divisors, size_t structSize = -1, std::string structName = "unspecified") {
	const size_t maxSize = 16;
	size_t offset = 0;
	for (size_t i = 0; i < locInfo.size(); i++) {
		if (locInfo[i].size > maxSize) {

			// TODO - this is an untested DSA implementation for instancing

			size_t locNr = locInfo[i].size / maxSize;
			for (size_t j = 0; j < locNr; j++) {
				bindDebugMessage(locInfo[i].loc + j, locInfo[i].name, locInfo[i].size / maxSize, structSize, (offset + (j * maxSize)), structName);

				glEnableVertexArrayAttrib(VAO, locInfo[i].loc + j);
				//glVertexAttribPointer(locInfo[i].loc + j, locInfo[i].size / maxSize, GL_FLOAT, GL_FALSE, sizeof(Store), (void*)(offset + (j * maxSize)));
				glVertexArrayAttribFormat(VAO, locInfo[i].loc, locInfo[i].size / maxSize, GL_FLOAT, GL_FALSE, offset + (j * maxSize));
				glVertexArrayAttribBinding(VAO, locInfo[i].loc + j, bindIndex());
				setVertexDivisors(locInfo[i].loc, j, divisors);
			}
		}
		else {
			bindDebugMessage(locInfo[i].loc, locInfo[i].name, locInfo[i].size / sizeof(float), structSize, offset, structName);
			glEnableVertexArrayAttrib(VAO, locInfo[i].loc);
			//glVertexAttribPointer(locInfo[i].loc, locInfo[i].size / sizeof(float), GL_FLOAT, GL_FALSE, sizeof(Store), (void*)offset);
			glVertexArrayAttribFormat(VAO, locInfo[i].loc, locInfo[i].size / sizeof(float), GL_FLOAT, GL_FALSE, offset);
			glVertexArrayAttribBinding(VAO, locInfo[i].loc, bindIndex());
			setVertexDivisors(locInfo[i].loc, 0, divisors);
		}
		offset += locInfo[i].size;
	}
	//Make ShaderBinder able to return the size of the vertex struct currently in use.
	//MAYBE: Make Shader class take a parameter which defines what structure it should use.
	//Make VertexAttribute take a template argument taking an attribute object type, which will be what it generates attributes for.
}

inline void VertexArray::bindDebugMessage(size_t location, std::string name, size_t size, size_t stride, size_t offset, std::string structName) {
	debug("Vertex attribute pointer set with:\n\tlocation: " + std::to_string(location) + "\n\tsize: " + std::to_string(size) + "\n\tstride: " + std::to_string(stride) + "\n\toffset: " + std::to_string(offset) + "\n\tstruct: " + structName + "\n", "glVertexAttribPointer");
}

void VertexArray::setVertexDivisors(size_t location, size_t subLoc, LocationVector locDivs) {
	for (size_t i = 0; i < locDivs.size(); i++) {
		if (locDivs[i].loc == location) {
			glVertexArrayBindingDivisor(VAO, bindIndex(), 1);
			//glVertexAttribDivisor(location + subLoc, 1);
			debug("Attribute divisor set for location: " + std::to_string(location + subLoc) + "\n", "glVertexAttribDivisor");
		}
	}
}
