#pragma once

#include <glm/glm.hpp>


class Vertex
{
public:
	glm::vec3 aPos;
	glm::vec3 aNormal;
	glm::vec2 aTexCoords;

	static constexpr size_t offset[] = { 0,3,6 };

private:

};

