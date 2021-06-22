#include "Model.h"

Model::Model(std::string path)
{
	load(path);
}

void Model::draw()
{
	for (size_t i = 0; i < meshes.size(); i++) {
		meshes[i].draw();
	}
}

void Model::load(std::string path)
{
	Assimp::Importer modelImporter;
	const aiScene* scene = modelImporter.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "Assimp Error: " << modelImporter.GetErrorString() << std::endl;
	}
	directory = path.substr(0, path.find_last_of('/'));

	processNode(scene, scene->mRootNode);
}

void Model::processNode(const aiScene* scene, aiNode* node)
{
	for (size_t i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(scene, mesh));
	}

	for (size_t i = 0; i < node->mNumChildren; i++) {
		processNode(scene, node->mChildren[i]);
	}
}

Mesh Model::processMesh(const aiScene* scene, aiMesh* mesh)
{
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	for (size_t i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;

		vertex.position.x = mesh->mVertices[i].x;
		vertex.position.y = mesh->mVertices[i].y;
		vertex.position.z = mesh->mVertices[i].z;

		if (mesh->HasNormals()) {
			vertex.normal.x = mesh->mNormals[i].x;
			vertex.normal.y = mesh->mNormals[i].y;
			vertex.normal.z = mesh->mNormals[i].z;
		}

		if (mesh->mTextureCoords[0]) {
			vertex.textureCoordinates.x = mesh->mTextureCoords[0][i].x;
			vertex.textureCoordinates.y = mesh->mTextureCoords[0][i].y;
		}
		else {
			vertex.textureCoordinates = glm::vec2(0.0f, 0.0f);
		}

		vertices.push_back(vertex);
	}


	for (size_t i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++) {
			indices.push_back((face.mIndices[j]));
		}

		// Generate normals if not present - used for lighting on test objects.
		if (!mesh->HasNormals()) {
			Vertex* triangle[3];
			for (size_t j = 0; j < face.mNumIndices; j++) {
				triangle[j] = &vertices[face.mIndices[j]];
			}

			glm::vec3 normal = glm::triangleNormal(triangle[0]->position, triangle[1]->position, triangle[2]->position);
			for (Vertex* vPtr : triangle) {
				vPtr->normal.x = normal.x;
				vPtr->normal.y = normal.y;
				vPtr->normal.z = normal.z;
			}
		}
	}

	if (mesh->mMaterialIndex >= 0) {
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, Texture::TextureType::DIFFUSE);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, Texture::TextureType::SPECULAR);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	

	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* material, aiTextureType type, Texture::TextureType textureType)
{
	std::vector<Texture> textures;
	for (size_t i = 0; i < material->GetTextureCount(type); i++) {
		aiString string;
		material->GetTexture(type, i, &string);
		bool skip = false;
		for (size_t j = 0; j < Model::textures.size(); j++) {
			if (std::strcmp(Model::textures[j].path.data(), string.C_Str()) == 0) {
				textures.push_back(Model::textures[j]);
				skip = true;
				break;
			}
		}
		
		if (!skip) {
			Texture texture;
			texture.id = TextureFromFile(directory + '/' + string.C_Str());
			texture.type = textureType;
			texture.path = string.C_Str();
			textures.push_back(texture);
			Model::textures.push_back(texture);
		}
	}
	return textures;
}

uint32_t Model::TextureFromFile(const std::string path)
{
	uint32_t textureID;
	glGenTextures(1, &textureID);

	int32_t width, height, nrComponents;
	uint8_t* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
	if (data) {
		GLenum format{};

		switch (nrComponents) {
		case 1:
			format = GL_RED;
			break;
		case 3:
			format = GL_RGB;
			break;
		case 4: 
			format = GL_RGBA;
			break;
		}

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else {
		std::cout << "Could not load texture: " << path << std::endl;
	}

	return textureID;
}
