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


#include "ModelData.h"
#include "Texture.h"
#include "Shader.h"

#include "Renderable.h"

#include "DebugLogger.h"

class Model : public DebugLogger<Model>, public Renderable
{
public:
	
	Model(ModelData& data);

	glm::mat4 getModelMatrix();
	void rotate(float angle, glm::vec3 axis);
	void setRotation(float angle, glm::vec3 axis);
	void move(float x, float y, float z);
	void setPosition(float x, float y, float z);
	void scale(float x, float y, float z);
	void setScale(float x, float y, float z);

	void render(const Shader& shader) override;
private:

	ModelData* model;

	glm::mat4 modelScale;
	glm::mat4 modelTranslation;
	glm::mat4 modelRotation;

};

