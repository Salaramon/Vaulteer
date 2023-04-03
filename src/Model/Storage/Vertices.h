#pragma once

#include <vector>
#include <memory>

#include "MaterialVertex.h"
#include "Vertex.h"


class Vertices {
public:
	Vertices() {}

	void add(Vertex& vertex) {
		vertices.push_back(std::make_unique<Vertex>(std::move(vertex)));
	}


private:
	std::vector<MaterialVertex> vertices;

};
