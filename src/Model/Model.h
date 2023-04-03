#pragma once

#include <glm/glm.hpp>

#include "Model/Data/ModelData.h"
#include "Renderer/Shader.h"
#include "Scene/Object3D.h"

template<class Data>
class Model : public Object3D
{
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

	Model(Data& data, int instances = 1);
	Model(Data* data, int instances = 1);

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

	
private:
	Data* model = nullptr;
	Material* materialOverride = nullptr;

	GLenum polygonFaces = Faces::Both;
	GLenum polygonMode = Polygon::Fill;
	GLfloat polygonLineWidth = 1;


public:
	inline static auto CR = DY::ClassRegister<
		&setPolygonFaces,
		&setPolygonMode,
		&setPolygonLineWidth,
		&getBoundingSphere,
		&getPolygonFaces,
		&getPolygonMode,
		&getPolygonLineWidth,
		&getData>(
			"setPolygonFaces",
			"setPolygonMode",
			"setPolygonLineWidth",
			"getBoundingSphere",
			"getPolygonFaces",
			"getPolygonMode",
			"getPolygonLineWidth",
			"getData");

	DY::ObjectRegister<Model<Data>,
		decltype(model),
		decltype(polygonFaces),
		decltype(polygonMode)> OR;

	inline static auto CB = DY::ClassBinder(CR);
	inline static auto OB = DY::ObjectBinder<decltype(OR), decltype(Shader::OB)>();

	using LOG = _LOG<decltype(CB), decltype(OB), DY::No_FB, DY::No_VB>;
};

template <class Data>
Model<Data>::Model(Model&& model) noexcept :
	Object3D(std::forward<Model>(model)),
	model(std::move(model.model)),
	polygonFaces(std::move(model.polygonFaces)),
	polygonMode(std::move(model.polygonMode)),
	polygonLineWidth(std::move(model.polygonLineWidth)) {}

template<class Data>
Model<Data>::Model(Data& data, int instances) :
	model(&data),

	OR(this,
	   DY::V(
		   &model,
		   &polygonFaces,
		   &polygonMode),
	   DY::N(
		   "model",
		   "polygonFaces",
		   "polygonMode")) {
	OB.add(OR);

	if (instances > 1) {
		data.getMeshes();
	}

	LOG::CTOR::debug(this, "Model data loaded into model from reference");
}

template<class Data>
Model<Data>::Model(Data* data, int instances) :
	model(data),

	OR(this,
	   DY::V(
		   &model,
		   &polygonFaces,
		   &polygonMode),
	   DY::N(
		   "model",
		   "polygonFaces",
		   "polygonMode")) {
	OB.add(OR);

	LOG::CTOR::debug(this, "Model data loaded into model from pointer");
}

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
	LOG::CLAS::debug<&Model<Data>::getBoundingSphere>(this, std::format("Got bounding sphere with values: {}", glm::to_string(sphere)));
	return sphere;
}
