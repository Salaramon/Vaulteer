#include "vpch.h"
#include "Model/Data/ModelData.h"

/*ModelData::ModelData(GLsizei width, GLsizei height, std::vector<glm::vec4> colors, std::vector<Vertex> vertices) {
	Vertices vert;
	Indices indi;
	storePointsAndIndices<ModelVertexHash>(indi, vert, vertices);
	meshes.emplace_back(vert, indi);
}*/

ModelData::ModelData(const std::string& modelPath, std::vector<Mesh>& meshVec) :
	modelPath(modelPath),
	textureID(-1),
	meshes(std::move(meshVec)) {
	std::cout << "Loading model: " + modelPath + "\n" << std::endl;
}

ModelData::ModelData(ModelData&& other) noexcept :
	modelPath(std::move(other.modelPath)),
	textureID(-1),
	meshes(std::move(other.meshes)),
	unitByTexturePath(std::move(other.unitByTexturePath)) {
	std::cout << "Model moved: " << modelPath << std::endl;
};


std::vector<Mesh>& ModelData::getMeshes() {
	return meshes;
}

GLint ModelData::getTextureID() const {
	return textureID;
}

const ModelData::ModelUnitTable& ModelData::getModelUnitTable() const {
	return modelUnitTable;
}

glm::vec4 ModelData::getBoundingSphere() {
	std::vector<Seb::Point<double>> points;
	for (const Mesh& m : meshes) {
		for (const Vertex& v : m.vertices) {
			std::vector<double> converter({ v.aPos.x, v.aPos.y, v.aPos.z });
			points.emplace_back(3, converter.begin());
		}
	}
	Seb::Smallest_enclosing_ball<double> ball(3, points);
	Seb::Smallest_enclosing_ball<double>::Coordinate_iterator it = ball.center_begin();

	glm::vec4 data(it[0], it[1], it[2], ball.radius());

	return data;
}


void ModelData::updateWithTextureUnits(const Texture2DArray& texture) {
	this->textureID = texture.getTextureID();

	for (auto& mesh : meshes) {
		auto& locators = mesh.material.textureTypeLocators;
		if (locators.empty())
			continue;

		auto& [path, type] = locators.at(aiTextureType_DIFFUSE);
		Texture2DArray::TextureUnit diffuseUnit = texture.getUnit(path);

		// map vertices' texture coordinates to diffuse texture unit in packed library
		for (auto& vertex : mesh.vertices) {
			vertex.aTexCoords.x = vertex.aTexCoords.x * static_cast<float>(diffuseUnit.w) / texture.width + static_cast<float>(diffuseUnit.x) / texture.width;
			vertex.aTexCoords.y = vertex.aTexCoords.y * static_cast<float>(diffuseUnit.h) / texture.height + static_cast<float>(diffuseUnit.y) / texture.height;
		}
		mesh.updateBuffer();

		// set material units to relative units for every material but diffuse
		for (const auto& entry : locators) {
			if (entry.first == aiTextureType_DIFFUSE) {
				unitByTexturePath[path] = diffuseUnit;
				modelUnitTable.setUnit(aiTextureType_DIFFUSE, diffuseUnit);
				continue;
			}
			TextureResourceLocator trl = entry.second;

			Texture2DArray::TextureUnit unit = texture.getUnit(trl.path);
			unit = unit.minus(diffuseUnit);
			unitByTexturePath[trl.path] = unit;
			modelUnitTable.setUnit(entry.first, unit);
		}
	}
}
