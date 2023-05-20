#pragma once

#include "Renderer/Batch.h"

class BatchManager {
public:
	constexpr static inline size_t default_vertex_buffer_size = 4000000 / sizeof(VertexImpl);
	constexpr static inline size_t default_index_buffer_size = 1000000 / sizeof(GLuint);

	BatchManager() = default;

	void setTextureID(GLint textureID) {
		this->textureID = textureID;
	}

	void clear() {
		batches.clear();
	}

	void createBatch(size_t vbSize = default_vertex_buffer_size, size_t ibSize = default_index_buffer_size) {
		batches.push_back(std::make_unique<Batch>(textureID, vbSize, ibSize));
	}

	void addToBatch(Mesh& mesh, glm::mat4 modelMat) {
		if (batches.empty()) {
			createBatch();
		}

		if (Batch* batch = batches.back().get(); !batch->add(mesh, modelMat)) {
			createBatch();
			batches.back()->add(mesh, modelMat);
		}
	}

	std::vector<Batch*> getBatches() {
		std::vector<Batch*> batchViews;
		for (auto& batch : batches) {
			batchViews.push_back(batch.get());
		}
		return batchViews;
	}

	std::vector<std::unique_ptr<Batch>> batches;

private:
	GLint textureID = 0;
};
