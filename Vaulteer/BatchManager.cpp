#include "vpch.h"
#include "BatchManager.h"

void BatchManager::setTextureID(BatchManager& manager, GLint textureID) {
	manager.textureID = textureID;
}

void BatchManager::addToBatch(BatchManager& manager, const Mesh& mesh, glm::mat4 modelMat) {
	auto& batches = manager.batches;
	if (batches.size() == 0) {
		batches.push_back(std::make_unique<Batch>(manager.textureID, default_vertex_buffer_size, default_index_buffer_size));
	}

	Batch* batch = batches.back().get();
	if (!batch->add(mesh, modelMat)) {
		batches.push_back(std::make_unique<Batch>(manager.textureID, default_vertex_buffer_size, default_index_buffer_size));
		batches.back()->add(mesh, modelMat);
	}
}

std::vector<std::reference_wrapper<Batch>> BatchManager::getBatches(BatchManager& manager) {
	std::vector<std::reference_wrapper<Batch>> batchViews;
	for (const auto& batch : manager.batches) {
		batchViews.push_back(std::ref(*batch.get()));
	}
	return batchViews;
}
