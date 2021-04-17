#pragma once

#include <vector>
#include <string>
#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"

#include "Renderable.h"
#include "Mesh.h"

class Model : public Renderable
{
public:

	Model(std::string path);

private:
	void renderingLogic() override;

	std::vector<Texture> textures;
	std::vector<Mesh> meshes;
	std::string directory;

	void load(std::string path);
	void processNode(const aiScene* scene, aiNode* node);
	Mesh&& processMesh(const aiScene* scene, aiMesh* mesh);

	std::vector<Texture> loadMaterialTextures(aiMaterial* material, aiTextureType type, Texture::TextureType textureType);
	uint32_t TextureFromFile(const std::string path);
};

