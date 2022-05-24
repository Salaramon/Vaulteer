#include "vpch.h"
#include "BatchManager.h"

void BatchManager::setTextureID(GLint textureID) {
	this->textureID = textureID;
}

void BatchManager::addToBatch(const Mesh& mesh, glm::mat4 modelMat) {
	if (batches.size() == 0) {
		batches.push_back(std::make_unique<Batch>(textureID, default_vertex_buffer_size, default_index_buffer_size));
	}

	Batch* batch = batches.back().get();
	if (!batch->add(mesh, modelMat)) {
		batches.push_back(std::make_unique<Batch>(textureID, default_vertex_buffer_size, default_index_buffer_size));
		batches.back()->add(mesh, modelMat);
	}
}

std::vector<std::reference_wrapper<Batch>> BatchManager::getBatches() {
	std::vector<std::reference_wrapper<Batch>> batchViews;
	for (const auto& batch : batches) {
		batchViews.push_back(std::ref(*batch.get()));
	}
	return batchViews;
}
