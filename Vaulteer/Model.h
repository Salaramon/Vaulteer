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

#include "Renderable.h"

#include "DebugLogger.h"

class Model : public DebugLogger<Model>, public Renderable
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
	Model(GraphicsData& data);
	Model(ModelData& data);
	Model(LineData& data);

	glm::mat4 getModelMatrix();
	void rotate(float angle, glm::vec3 axis);
	void setRotation(float angle, glm::vec3 axis);
	void move(float x, float y, float z);
	void setPosition(float x, float y, float z);
	void scale(float x, float y, float z);
	void setScale(float x, float y, float z);
	void setPolygonFaces(GLenum faces);
	void setPolygonMode(GLenum mode);
	void setPolygonLineWidth(GLfloat width);

	GLenum getPolygonFaces();
	GLenum getPolygonMode();
	GLfloat getPolygonLineWidth();

	GraphicsData* getData();

	void render(const Shader& shader) override;
private:

	GraphicsData* model = nullptr;

	glm::mat4 modelScale;
	glm::mat4 modelTranslation;
	glm::mat4 modelRotation;

	GLenum polygonFaces = Faces::Both;
	GLenum polygonMode = Polygon::Fill;
	GLfloat polygonLineWidth = 1;

};

