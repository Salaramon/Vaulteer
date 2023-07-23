#pragma once

#include <string>

struct ModelResourceLocator {
	std::string name, path;
	bool useAdjacency, smoothNormals;
};
