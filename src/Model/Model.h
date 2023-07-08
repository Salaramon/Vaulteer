#pragma once

#

#include <glm/glm.hpp>
#include <Seb.h> 

#include "Model/Mesh.h"
#include "Textures/Texture2DArray.h"
#include "Scene/Object3D.h"
#include "Storage/VertexImpl.h"
#include "Resources/MaterialLibrary.h"
#include "VFX/LightTypes.h"


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


struct Opaque {
	bool transparent = false;
};
struct Transparent {
	bool transparent = true;
};

struct Shadow {
	bool shadow = true;
};

struct Instanced {
	bool instanced = true;
};


class Model : public Object3D {
public:
	Model() :
			meshes(&this->add<Meshes>()),
			propertiesModel(&this->add<PropertiesModel>(PropertiesModel{
				.boundingSphere = glm::vec4(1.0), //boundingSphere(),
				.faces = Faces::FRONT,
				.textureID = 0
			})),
			textureView(&this->add<TextureView>()) {}

	Model(std::vector<Mesh*>& meshes) :
			meshes(&this->add<Meshes>(meshes)),
			propertiesModel(&this->add<PropertiesModel>(PropertiesModel{
				.boundingSphere = glm::vec4(1.0), //boundingSphere(),
				.faces = Faces::FRONT,
				.textureID = 0
			})),
			textureView(&this->add<TextureView>()) {

		//addRenderComponents();
	}

	Model(Model& other) = delete;

	Model(Model&& other) noexcept :
			Object3D(std::move(other)),
			meshes(other.meshes),
			propertiesModel(other.propertiesModel),
			textureView(other.textureView) {
		other.meshes = nullptr;
		other.propertiesModel = nullptr;
		other.textureView = nullptr;
	}

	~Model() {
		this->remove<Meshes, PropertiesModel, TextureView>();
	}

	Meshes* meshes;
	PropertiesModel* propertiesModel;
	TextureView* textureView;

	void setPolygonFaces(Faces faces) {
		propertiesModel->faces = faces;
	}

	void setMaterial(Material* mat) {
		for (auto* mesh : *meshes) {
			mesh->overrideMaterial(mat);
		}
	}

	void setMaterial(Material* mat, int meshIndex) {
		meshes->at(meshIndex)->overrideMaterial(mat);
	}

	void addRenderComponents() {
		for (auto* mesh : *meshes) {
			if (mesh->material->isTransparent()) {
				this->add<Transparent>();
				break;
			}
		}
	}

	void useInstancing(const std::vector<glm::mat4>& instanceMats) {
		for (auto* mesh : *meshes) {
			mesh->insertInstances(instanceMats);
		}
		this->add<Instanced>();
	}
	
private:
	glm::vec4 boundingSphere() {
		std::vector<Seb::Point<double>> points;
		for (Mesh* mesh : *meshes) {
			for (const VertexImpl& vertex : mesh->getCopiedData<VertexImpl>()) {
				std::vector<double> converter({ vertex.aPos.x, vertex.aPos.y, vertex.aPos.z });
				points.emplace_back(3, converter.begin());
			}
		}

		glm::vec4 boundingSphere{0, 0, 0, 0};

		if (!points.empty()) {
			Seb::Smallest_enclosing_ball<double> ball(3, points);
			Seb::Smallest_enclosing_ball<double>::Coordinate_iterator it = ball.center_begin();

			boundingSphere = glm::vec4(it[0], it[1], it[2], ball.radius());

			boundingSphere.x += position->x;
			boundingSphere.y += position->y;
			boundingSphere.z += position->z;
		}

		return boundingSphere;
	}
};
