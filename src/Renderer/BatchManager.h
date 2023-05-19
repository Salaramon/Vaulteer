#pragma once

#include "Renderer/Batch.h"

class BatchManager {
public:
	constexpr static inline size_t default_vertex_buffer_size = 4000000 / sizeof(VertexImpl);
	constexpr static inline size_t default_index_buffer_size = 1000000 / sizeof(GLuint);

	BatchManager() = default;

	static void setTextureID(BatchManager& manager, GLint textureID);

	static void addToBatch(BatchManager& manager, Mesh& mesh, glm::mat4 modelMat);

	static std::vector<std::reference_wrapper<Batch>> getBatches(const BatchManager& manager);

	std::vector<std::unique_ptr<Batch>> batches;

private:
	GLint textureID = 0;
};
