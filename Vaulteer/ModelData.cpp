#include "ModelData.h"

ModelData::ModelData(GLsizei width, GLsizei height, std::vector<glm::vec4> colors, std::vector<Vertex> vertices) {
	Vertices vert;
	Indices indi;
	storePointsAndIndices<ModelVertexHash>(indi, vert, vertices);
	meshes.emplace_back(vert, indi);
}

ModelData::ModelData(std::string modelPath, std::vector<Mesh>& meshVec) : modelPath(modelPath), meshes(std::move(meshVec)) {
	std::cout << "Loading model: " + modelPath + "\n" << std::endl;
	/*meshes->insert(meshes->end(),
		std::make_move_iterator(meshVec->begin()),
		std::make_move_iterator(meshVec->end())
	);*/
}

ModelData::ModelData(ModelData&& other) noexcept :
	modelPath(other.modelPath),
	meshes(std::move(other.meshes)),
	unitByTexturePath(std::move(other.unitByTexturePath)) {
	std::cout << "Model moved: " << modelPath << std::endl;
};


std::vector<Mesh>& ModelData::getMeshes() {
	return meshes;
}

const GLint ModelData::getTextureID() const {
	return textureID;
}

const ModelData::ModelUnitTable& ModelData::getModelUnitTable() const {
	return modelUnitTable;
}

void ModelData::updateWithTextureUnits(const Texture2DArray& texture) {
	this->textureID = texture.getTextureID();

	for (auto& mesh : meshes) {
		auto& locators = mesh.material.textureTypeLocators;
		if (locators.size() == 0)
			continue;

		auto& locator = locators.at(aiTextureType_DIFFUSE);
		Texture2DArray::TextureUnit diffuseUnit = texture.getUnit(locator.path);

		// map vertices' texture coordinates to diffuse texture unit in packed library
		for (auto& vertex : mesh.vertices) {
			vertex.aTexCoords.x = vertex.aTexCoords.x * (double)diffuseUnit.w / texture.width  + (double)diffuseUnit.x / texture.width;
			vertex.aTexCoords.y = vertex.aTexCoords.y * (double)diffuseUnit.h / texture.height + (double)diffuseUnit.y / texture.height;
		}
		mesh.updateBuffer();

		// set material units to relative units for every material but diffuse
		for (const auto& entry : locators) {
			if (entry.first == aiTextureType_DIFFUSE) { 
				unitByTexturePath[locator.path] = diffuseUnit;
				modelUnitTable.setUnit(aiTextureType_DIFFUSE, diffuseUnit);
				continue; 
			}
			Texture2DArray::TextureResourceLocator locator = entry.second;

			Texture2DArray::TextureUnit unit = texture.getUnit(locator.path);
			unit = unit.minus(diffuseUnit);
			unitByTexturePath[locator.path] = unit;
			modelUnitTable.setUnit(entry.first, unit);
		}
	}
}
