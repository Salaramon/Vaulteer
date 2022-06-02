#pragma once

#include "Batch.h"

class BatchManager {
public:
	const static inline size_t default_vertex_buffer_size = 4000000 / sizeof(Vertex);
	const static inline size_t default_index_buffer_size = 1000000 / sizeof(Index);

	BatchManager() {}

	static void setTextureID(BatchManager& manager, GLint textureID);

	static void addToBatch(BatchManager& manager, const Mesh& model, glm::mat4 modelMat);

	static std::vector<std::reference_wrapper<Batch>> getBatches(BatchManager& manager);

private:
	std::vector<std::unique_ptr<Batch>> batches;

	GLint textureID = 0;
};
