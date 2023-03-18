#pragma once

#

#include <glm/glm.hpp>
#include <assimp/scene.h>

#include "Renderer/Shader.h"
#include "Scene/Object3D.h"
#include "Model/Mesh.h"
#include "Textures/Texture2DArray.h"

#include <Seb.h> 

enum class Faces {
	FRONT = GL_FRONT,
	BACK = GL_BACK,
	BOTH = GL_FRONT_AND_BACK
};

struct PropertiesModel {
public:
	glm::vec4 boundingSphere;
	Faces faces;
	size_t textureID;
};

struct Meshes : public std::vector<Mesh*> {
public:
	using std::vector<Mesh*>::operator=;
	using std::vector<Mesh*>::vector;
};

struct ModelUnitTable {
public:
	struct ModelUnitData {
		int xDelta, yDelta, wDelta, hDelta, layerDelta;
		ModelUnitData() : xDelta(0), yDelta(0), wDelta(0), hDelta(0), layerDelta(1) {}
		ModelUnitData(const Texture2DArray::TextureUnit& u) : xDelta(u.x), yDelta(u.y), wDelta(u.w), hDelta(u.h), layerDelta(u.layer) {}
	};

	ModelUnitData diffuseUnit, specularUnit, normalMapUnit;

	ModelUnitTable() = default;

	void setUnit(aiTextureType ttype, const Texture2DArray::TextureUnit& u) {
		switch (ttype) {
		case aiTextureType_DIFFUSE: diffuseUnit = u;
			break;
		case aiTextureType_SPECULAR: specularUnit = u;
			break;
		case aiTextureType_HEIGHT: normalMapUnit = u;
			break;
		default:;
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

template<class... TupleArgs>
class ModelUtility : public Entity::Restricter<TupleArgs...> {
public:
	ModelUtility(TupleArgs... args) :
		meshes(Entity::tryGet<Meshes>(std::tie(args...))),
		propertiesModel(Entity::tryGet<PropertiesModel>(std::tie(args...))),
		modelUnitTable(Entity::tryGet<ModelUnitTable>(std::tie(args...)))
	{}

	ModelUtility(std::tuple<TupleArgs...> tuple) :
		meshes(Entity::tryGet<Meshes>(tuple)),
		propertiesModel(Entity::tryGet<PropertiesModel>(tuple)),
		modelUnitTable(Entity::tryGet<ModelUnitTable>(tuple))
	{}

	

protected:
	Meshes* meshes;
	PropertiesModel* propertiesModel;
	ModelUnitTable* modelUnitTable;
};

class Model : public Object3D {
public:

	Model() :
		meshes(this->add<Meshes>()),
		propertiesModel(this->add<PropertiesModel>(PropertiesModel{
			.boundingSphere = boundingSphere(),
			.faces = Faces::FRONT,
			.textureID = 0
			})),
		modelUnitTable(this->add<ModelUnitTable>())
	{}

	Meshes& meshes;
	PropertiesModel& propertiesModel;
	ModelUnitTable& modelUnitTable;

	void setPolygonFaces(Faces faces) {
		propertiesModel.faces = faces;
	}
	
private:

	glm::vec4 boundingSphere() {
		std::vector<Seb::Point<double>> points;
		for (const Mesh* mesh : meshes) {
			for (const Vertex& vertex : mesh->vertices) {
				std::vector<double> converter({ vertex.aPos.x, vertex.aPos.y, vertex.aPos.z });
				points.emplace_back(3, converter.begin());
			}
		}
		Seb::Smallest_enclosing_ball<double> ball(3, points);
		Seb::Smallest_enclosing_ball<double>::Coordinate_iterator it = ball.center_begin();

		glm::vec4 boundingSphere(it[0], it[1], it[2], ball.radius());

		boundingSphere.x += position.x;
		boundingSphere.y += position.y;
		boundingSphere.z += position.z;

		return boundingSphere;
	}
};