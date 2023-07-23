#pragma once

#include <assimp/scene.h>

#include "Model/Storage/Geometry.h"
#include "Model/Material.h"

class GeometryLibrary {
public:
	inline static std::vector<std::unique_ptr<Geometry>> geometryLibrary;
	inline static std::unordered_map<std::string, size_t> geometryIndexByPath;

	inline static size_t numGeometries = 0;

	// creates and stores geometry by the given name and returns a pointer view to be used with a mesh
	template<vertex_concept T>
	static Geometry* create(const std::string name, const std::vector<T> vertices, std::vector<GLuint> indices, GLenum type) {
		auto it = geometryIndexByPath.find(name);
		if (it != geometryIndexByPath.end())
			return geometryLibrary[(*it).second].get();

		auto geometry = std::make_unique<Geometry>(vertices, indices, type);
		geometryLibrary.push_back(std::move(geometry));

		auto [geometryByPath, inserted] = geometryIndexByPath.emplace(name, numGeometries);
		KYSE_ASSERT(inserted)

		numGeometries++;
		return geometryLibrary.back().get();
	}

	static Geometry* get(unsigned int index) {
		KYSE_ASSERT(index < numGeometries)
		return geometryLibrary[index].get();
	}

	static Geometry* get(const std::string& name) {
		auto it = geometryIndexByPath.find(name);
		if (it != geometryIndexByPath.end()) {
			return geometryLibrary[(*it).second].get();
		}
		return nullptr;
	}

	static size_t size() {
		return geometryLibrary.size();
	}

private:
	// this class is static only
	GeometryLibrary() = default;
};