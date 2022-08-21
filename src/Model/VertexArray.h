#pragma once

#include <vector>
#include <iostream>

#include <glad/glad.h>

#include "Model/Storage/Vertex.h"
#include "GLSLCPPBinder.h"

typedef std::vector<Vertex> Vertices;
typedef std::vector<GLuint> Indices;

template<class Store>
class VertexArray {
public:
	VertexArray();
	VertexArray(VertexArray<Store>&& other) :
		VAO(other.VAO) {
		other.VAO = 0;
		//debug("VertexArray moved. VAO: " + std::to_string(VAO) + "\n", "MOVE_CONSTRUCTOR");

	}
	~VertexArray();
	operator GLuint() const { return VAO; }

	void bind() const;
	void unbind() const;

	/* Binding index for VBO -> binding -> VAO API. Implemented as just the VAO buffer ID */
	GLuint bindIndex() const;

	void setUpAttributes(LocationVector locInfo, LocationVector divisors, size_t structSize, std::string structName);

private:
	void cleanup();
	void initialize();

	void setVertexDivisors(size_t location, size_t subLoc, LocationVector locDivs);

	void bindDebugMessage(size_t location, std::string name, size_t size, size_t stride, size_t offset, std::string structName);

	GLuint VAO;

	bool setUp = false;
};

template<class Store>
VertexArray<Store>::VertexArray() {
	initialize();
	//debug("VertexArray created VAO: " + std::to_string(VAO) + "\n", "DEFAULT_CONSTRUCTOR");
}

template<class Store>
VertexArray<Store>::~VertexArray() {
	cleanup();
	//debug("VertexArray destroyed. VAO: " + std::to_string(VAO) + "\n", "DECONSTRUCTOR");
}

template<class Store>
inline void VertexArray<Store>::bind() const {
	glBindVertexArray(VAO);
}

template<class Store>
inline void VertexArray<Store>::unbind() const {
	glBindVertexArray(0);
}

template<class Store>
inline GLuint VertexArray<Store>::bindIndex() const {
	return 0; // VAO - 1;
}

template<class Store>
inline void VertexArray<Store>::initialize() {
	glCreateVertexArrays(1, &VAO);
}

template<class Store>
inline void VertexArray<Store>::cleanup() {
	glDeleteVertexArrays(1, &VAO);
}

template<class Store>
inline void VertexArray<Store>::setUpAttributes(LocationVector locInfo, LocationVector divisors, size_t structSize, std::string structName) {
	if (setUp) 
		return;

	//Make a class that can be inherited into a Store class which then can be used to create vertex classes and the code below becomes more type safe.
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
	setUp = true;
	//Make ShaderBinder able to return the size of the vertex struct currently in use.
	//MAYBE: Make Shader class take a parameter which defines what structure it should use.
	//Make VertexAttribute take a template argument taking an attribute object type, which will be what it generates attributes for.
}

template<class Store>
inline void VertexArray<Store>::bindDebugMessage(size_t location, std::string name, size_t size, size_t stride, size_t offset, std::string structName) {
	//debug("Vertex attribute pointer set with:\n\tlocation: " + std::to_string(location) + "\n\tsize: " + std::to_string(size) + "\n\tstride: " + std::to_string(stride) + "\n\toffset: " + std::to_string(offset) + "\n\tstruct: " + structName + "\n", "glVertexAttribPointer");
}

template<class Store>
inline void VertexArray<Store>::setVertexDivisors(size_t location, size_t subLoc, LocationVector locDivs) {
	for (size_t i = 0; i < locDivs.size(); i++) {
		if (locDivs[i].loc == location) {
			glVertexArrayBindingDivisor(VAO, bindIndex(), 1);
			//glVertexAttribDivisor(location + subLoc, 1);
			//debug("Attribute divisor set for location: " + std::to_string(location + subLoc) + "\n", "glVertexAttribDivisor");
		}
	}
}
