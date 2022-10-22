#pragma once

#include <glm/glm.hpp>

#include "Model/Data/ModelData.h"
#include "Renderer/Shader.h"
#include "Scene/Object3D.h"

#include "Debug/DebugLogger.h"

template <class Data>
class Model : public DebugLogger<Model<char>>, public Object3D {
public:
	struct Faces {
		inline static constexpr GLenum Front = GL_FRONT;
		inline static constexpr GLenum Back = GL_BACK;
		inline static constexpr GLenum Both = GL_FRONT_AND_BACK;
	};

	struct Polygon {
		inline static constexpr GLenum Point = GL_POINT;
		inline static constexpr GLenum Line = GL_LINE;
		inline static constexpr GLenum Fill = GL_FILL;
	};

	Model(Data& data);
	Model(Data* data);
	Model(Model&) = delete;
	Model(Model&& model) noexcept;

	void setPolygonFaces(GLenum faces);
	void setPolygonMode(GLenum mode);
	void setPolygonLineWidth(GLfloat width);

	glm::vec4 getBoundingSphere();

	GLenum getPolygonFaces() const;
	GLenum getPolygonMode() const;
	GLfloat getPolygonLineWidth() const;

	Data* getData();
	Material* getMaterial();

	//void render(const Shader& shader) override;
private:
	Data* model = nullptr;
	Material* materialOverride = nullptr;

	GLenum polygonFaces = Faces::Both;
	GLenum polygonMode = Polygon::Fill;
	GLfloat polygonLineWidth = 1;
};

template <class Data>
Model<Data>::Model(Model&& model) noexcept :
	Object3D(std::forward<Model>(model)),
	model(std::move(model.model)),
	polygonFaces(std::move(model.polygonFaces)),
	polygonMode(std::move(model.polygonMode)),
	polygonLineWidth(std::move(model.polygonLineWidth)) {}

template <class Data>
Model<Data>::Model(Data& data) :
	Object3D(),
	model(&data) {}

template <class Data>
Model<Data>::Model(Data* data) :
	Object3D(),
	model(data) {}

template <class Data>
void Model<Data>::setPolygonFaces(GLenum faces) {
	polygonFaces = faces;
}

template <class Data>
void Model<Data>::setPolygonMode(GLenum mode) {
	polygonMode = mode;
}

template <class Data>
void Model<Data>::setPolygonLineWidth(GLfloat width) {
	polygonLineWidth = width;
}

template <class Data>
GLenum Model<Data>::getPolygonFaces() const {
	return polygonFaces;
}

template <class Data>
GLenum Model<Data>::getPolygonMode() const {
	return polygonMode;
}

template <class Data>
GLfloat Model<Data>::getPolygonLineWidth() const {
	return polygonLineWidth;
}

template <class Data>
Data* Model<Data>::getData() {
	return model;
}

template <class Data>
Material* Model<Data>::getMaterial() {
	return materialOverride != nullptr ? materialOverride : model->getOriginalMaterial() ;
}

template <class Data>
glm::vec4 Model<Data>::getBoundingSphere() {
	glm::vec4 sphere(model->getBoundingSphere());
	glm::vec3 pos = getPosition();
	sphere.x += pos.x;
	sphere.y += pos.y;
	sphere.z += pos.z;
	return sphere;
}
