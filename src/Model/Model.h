#pragma once

#

#include <glm/glm.hpp>
#include <assimp/scene.h>

#include "Renderer/Shader.h"
#include "Scene/Object3D.h"
#include "Model/Mesh.h"
#include "Textures/Texture2DArray.h"

#include <Seb.h> 

#include "Storage/MaterialVertex.h"

enum class Faces {
	FRONT = GL_FRONT,
	BACK = GL_BACK,
	BOTH = GL_FRONT_AND_BACK
};

struct PropertiesModel {
	glm::vec4 boundingSphere;
	Faces faces;
	size_t textureID;
};

struct Meshes : std::vector<Mesh*> {
	using std::vector<Mesh*>::operator=;
	using std::vector<Mesh*>::vector;
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
		textureView(this->add<TextureView>())
	{}

	Meshes& meshes;
	PropertiesModel& propertiesModel;
	TextureView& textureView;

	void setPolygonFaces(Faces faces) {
		propertiesModel.faces = faces;
	}
	
private:

	glm::vec4 boundingSphere() {
		std::vector<Seb::Point<double>> points;
		for (Mesh* mesh : meshes) {
			for (const MaterialVertex& vertex : mesh->getCopiedData<MaterialVertex>()) {
				std::vector<double> converter({ vertex.aPos.x, vertex.aPos.y, vertex.aPos.z });
				points.emplace_back(3, converter.begin());
			}
		}

		glm::vec4 boundingSphere{0, 0, 0, 0};

		if (!points.empty()) {
			Seb::Smallest_enclosing_ball<double> ball(3, points);
			Seb::Smallest_enclosing_ball<double>::Coordinate_iterator it = ball.center_begin();

			boundingSphere = glm::vec4(it[0], it[1], it[2], ball.radius());

			boundingSphere.x += position.x;
			boundingSphere.y += position.y;
			boundingSphere.z += position.z;
		}

		return boundingSphere;
	}
};


template<class... TupleArgs>
class ModelUtility : public Object3DUtility<TupleArgs...>, public Entity::Restricter<TupleArgs...> {
public:
	ModelUtility(const Model& model) : Object3DUtility<TupleArgs...>(model),
		meshes(&model.meshes),
		propertiesModel(&model.propertiesModel),
		modelUnitTable(&model.textureView)
	{}

	ModelUtility(TupleArgs... args) : Object3DUtility<TupleArgs...>(args...),
		meshes(Entity::tryGet<Meshes>(std::tie(args...))),
		propertiesModel(Entity::tryGet<PropertiesModel>(std::tie(args...))),
		modelUnitTable(Entity::tryGet<TextureView>(std::tie(args...)))
	{}

	ModelUtility(std::tuple<TupleArgs...> tuple) : Object3DUtility<TupleArgs...>(tuple),
		meshes(Entity::tryGet<Meshes>(tuple)),
		propertiesModel(Entity::tryGet<PropertiesModel>(tuple)),
		modelUnitTable(Entity::tryGet<TextureView>(tuple))
	{}


	const Meshes* const meshes;
	const PropertiesModel* const propertiesModel;
	const TextureView* const modelUnitTable;
};
