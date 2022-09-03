#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Model/Data/LineData.h"
#include "Model/Data/ModelData.h"
#include "Model/Textures/Texture.h"
#include "Renderer/Shader.h"
#include "Scene/Object3D.h"

template<class Data>
class Model : public Object3D
{
public:

	struct Faces {
		inline static const GLenum Front = GL_FRONT;
		inline static const GLenum Back = GL_BACK;
		inline static const GLenum Both = GL_FRONT_AND_BACK;
	};

	struct Polygon {
		inline static const GLenum Point = GL_POINT;
		inline static const GLenum Line = GL_LINE;
		inline static const GLenum Fill = GL_FILL;
	};

	Model(Model&) = delete;
	Model(Model&& model);
	Model(Data& data);
	Model(Data* data);

	void setPolygonFaces(GLenum faces);
	void setPolygonMode(GLenum mode);
	void setPolygonLineWidth(GLfloat width);

	glm::vec4 getBoundingSphere();

	GLenum getPolygonFaces();
	GLenum getPolygonMode();
	GLfloat getPolygonLineWidth();

	Data* getData();

	
private:

	Data* model = nullptr;

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

template<class Data>
inline Model<Data>::Model(Model&& model) :
	OR(model.OR),

	model(std::move(model.model)),
	polygonFaces(std::move(model.polygonFaces)),
	polygonMode(std::move(model.polygonMode)),
	polygonLineWidth(std::move(model.polygonLineWidth)),
	Object3D(std::forward<Model>(model))
{}

template<class Data>
inline Model<Data>::Model(Data& data) :
	OR(this,
		DY::V(
			&model,
			&polygonFaces,
			&polygonMode),
		DY::N(
			"model",
			"polygonFaces",
			"polygonMode")),

	Object3D(),
	model(&data)
{
	OB.add(OR);

	LOG::CTOR::debug(this, "Model data loaded into model from reference");
}

template<class Data>
inline Model<Data>::Model(Data* data) : 
	OR(this,
		DY::V(
			&model,
			&polygonFaces,
			&polygonMode),
		DY::N(
			"model",
			"polygonFaces",
			"polygonMode")),

	Object3D(),
	model(data)
{
	OB.add(OR);

	LOG::CTOR::debug(this, "Model data loaded into model from pointer");
}

template<class Data>
inline void Model<Data>::setPolygonFaces(GLenum faces)
{
	polygonFaces = faces;
}

template<class Data>
inline void Model<Data>::setPolygonMode(GLenum mode)
{
	polygonMode = mode;
}

template<class Data>
inline void Model<Data>::setPolygonLineWidth(GLfloat width)
{
	polygonLineWidth = width;
}

template<class Data>
inline GLenum Model<Data>::getPolygonFaces()
{
	return polygonFaces;
}

template<class Data>
inline GLenum Model<Data>::getPolygonMode()
{
	return polygonMode;
}

template<class Data>
inline GLfloat Model<Data>::getPolygonLineWidth()
{
	return polygonLineWidth;
}

template<class Data>
inline Data* Model<Data>::getData()
{
	return model;
}

template<class Data>
glm::vec4 Model<Data>::getBoundingSphere()
{
	glm::vec4 sphere(model->getBoundingSphere());
	glm::vec3 pos = getPosition();
	sphere.x += pos.x;
	sphere.y += pos.y;
	sphere.z += pos.z;
	LOG::CLAS::debug<&Model<Data>::getBoundingSphere>(this, std::format("Got bounding sphere with values: {}", glm::to_string(sphere)));
	return sphere;
}