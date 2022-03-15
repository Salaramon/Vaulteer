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

#include "LineData.h"
#include "ModelData.h"
#include "Texture.h"
#include "Shader.h"
#include "Object3D.h"

#include "DebugLogger.h"

template<class Data>
class Model : public DebugLogger<Model<char>>, public Object3D
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

	GLenum getPolygonFaces();
	GLenum getPolygonMode();
	GLfloat getPolygonLineWidth();

	Data* getData();

	//void render(const Shader& shader) override;
private:

	Data* model = nullptr;

	GLenum polygonFaces = Faces::Both;
	GLenum polygonMode = Polygon::Fill;
	GLfloat polygonLineWidth = 1;

};

template<class Data>
inline Model<Data>::Model(Model&& model) :
	model(std::move(model.model)),
	polygonFaces(std::move(model.polygonFaces)),
	polygonMode(std::move(model.polygonMode)),
	polygonLineWidth(std::move(model.polygonLineWidth))
{}

template<class Data>
inline Model<Data>::Model(Data& data) : 
	Object3D(),
	model(&data)
{}

template<class Data>
inline Model<Data>::Model(Data* data) : 
	Object3D(),
	model(data)
{}

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

