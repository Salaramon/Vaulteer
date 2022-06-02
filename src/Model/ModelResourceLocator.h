#pragma once

#include <string>

struct ModelResourceLocator {
	std::string name, path;
	int importFlags = -1;
};