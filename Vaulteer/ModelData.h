#pragma once

#include <vector>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <set>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GraphicsData.h"

#include "Texture2DArray.h"
#include "VertexHash.h"
#include "Mesh.h"

#include "DebugLogger.h"


class ModelData : public DebugLogger<ModelData>, public GraphicsData
{
public:
	struct ModelVertexHash : public VertexHash<Vertex> {
	public:
		size_t hash_combine(size_t lhs, size_t rhs) const {
			lhs ^= rhs + 0x9e3779b9 + (lhs << 6) + (lhs >> 2);
			return lhs;
		}

		bool operator()(const Vertex& a, const Vertex& b)const override
		{
			return a.aPos.x == b.aPos.x && a.aPos.y == b.aPos.y && a.aPos.z == b.aPos.z;
		}

		size_t operator()(const Vertex& k) const override
		{
			return hash_combine(
				hash_combine(std::hash<float>()(k.aPos.x), std::hash<float>()(k.aPos.y)),
				std::hash<float>()(k.aPos.z));
		}
	};

	struct ModelUnitData {
		int xDelta, yDelta, wDelta, hDelta, layerDelta;
		ModelUnitData() : xDelta(0), yDelta(0), wDelta(0), hDelta(0), layerDelta(1) {}
		ModelUnitData(Texture2DArray::TextureUnit& u) : xDelta(u.x), yDelta(u.y), wDelta(u.w), hDelta(u.h), layerDelta(u.layer) {}
	};
	struct ModelUnitTable {
		ModelUnitData diffuseUnit, specularUnit, normalMapUnit;

		ModelUnitTable() {}
		void setUnit(aiTextureType ttype, Texture2DArray::TextureUnit& u) {
			switch(ttype) {
			case aiTextureType_DIFFUSE: diffuseUnit = u; break;
			case aiTextureType_SPECULAR: specularUnit = u; break;
			case aiTextureType_HEIGHT: normalMapUnit = u; break;
			}
		}

		const ModelUnitData& operator[](size_t index) const {
			switch (index) {
			case 0: return diffuseUnit;
			case 1: return specularUnit;
			case 2: return normalMapUnit;
			default: throw std::exception("invalid index");
			}
		}
	};

	ModelData(GLsizei width, GLsizei height, std::vector<glm::vec4> colors, std::vector<Vertex> vertices);
	ModelData(std::string modelPath, std::vector<Mesh>& meshVec);
	ModelData(ModelData&& other) noexcept;

	std::vector<Mesh>& getMeshes();
	const GLint getTextureID() const;
	const ModelUnitTable& getModelUnitTable() const;

	void updateWithTextureUnits(const Texture2DArray& texture);

private:
	std::string modelPath;
	GLint textureID;

	// populated on load (loadModel)
	std::vector<Mesh> meshes;
	
	// populated after texture packing (updateWithTextureUnits)
	std::unordered_map<std::string, Texture2DArray::TextureUnit> unitByTexturePath;
	ModelUnitTable modelUnitTable;

};