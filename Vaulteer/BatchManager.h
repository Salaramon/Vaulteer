#pragma once

#include "Batch.h"

class BatchManager {
public:
	const static inline size_t default_vertex_buffer_size = 4000000 / sizeof(Vertex);
	const static inline size_t default_index_buffer_size = 1000000 / sizeof(Index);

	BatchManager() {};

	void setTextureID(GLint textureID);

	void addToBatch(const Mesh& model, glm::mat4 modelMat);

	std::vector<std::reference_wrapper<Batch>> getBatches();

private:
	std::vector<std::unique_ptr<Batch>> batches;

	GLint textureID = 0;
};
