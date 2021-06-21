#include "Model.h"

Model::Model(std::string meshPath)
{

	loadModel(meshPath);
}

Model::Model(std::string meshPath, std::string textureFolderPath)
{
	setTexturesFolder(textureFolderPath);
	loadModel(meshPath);
}

void Model::loadModel(std::string path)
{
	Assimp::Importer modelImporter;
	const aiScene* scene = modelImporter.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "Assimp Error: " << modelImporter.GetErrorString() << std::endl;
	}

	//directory = path.substr(0, path.find_last_of('/'));

	if (scene) {
		processNode(scene, scene->mRootNode);
	}
}

void Model::processNode(const aiScene* scene, aiNode* node)
{
	for (size_t i = 0; i < node->mNumMeshes; i++) {
		aiMesh* aiMesh = scene->mMeshes[node->mMeshes[i]];

		meshes.push_back(processMesh(scene, aiMesh));
	}

	for (size_t i = 0; i < node->mNumChildren; i++) {
		processNode(scene, node->mChildren[i]);
	}
}

Mesh&& Model::processMesh(const aiScene* scene, aiMesh* mesh)
{
	Vertices vertices;
	Indices indices;
	Textures textures;

	for (size_t i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		vertex.aPos = ai_glmVec(mesh->mVertices[i]);

		if (mesh->HasNormals()) {
			vertex.aNormal = ai_glmVec(mesh->mNormals[i]);
		}

		if (mesh->mTextureCoords[0]) {
			vertex.aTexCoords = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
		}
		else {
			vertex.aTexCoords = glm::vec2(0.0f, 0.0f);
		}

		vertices.push_back(vertex);
	}


	for (size_t i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++) {
			indices.push_back((face.mIndices[j]));
		}
	}

	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		getTextureUniforms(material);
	}



	return Mesh(vertices, indices);
}

void Model::getTextureUniforms(aiMaterial* material)
{
	for (size_t i = static_cast<size_t>(aiTextureType_NONE); i < static_cast<size_t>(aiTextureType_UNKNOWN) + 1; i++) {
		aiTextureType type = static_cast<aiTextureType>(i);
		for (size_t j = 0; j < material->GetTextureCount(type); j++) {
			aiString string;
			material->GetTexture(type, j, &string);
			std::string texturePath = texturesFolder + "\\" + string.C_Str();
			auto uniformTT_Iterator = Texture::uniformTextureTypes.find(type);
			if (textureFiles.insert(texturePath).second) {
				if (uniformTT_Iterator != Texture::uniformTextureTypes.end()) {
					textures.push_back(Texture(texturePath, uniformTT_Iterator->second));
				}
				else {
					std::cout << "Uniform for texture type " << type << " not available.\nSee assimp's aiTextureTypes." << std::endl;
				}
			}
		}
	}
}

glm::vec3 Model::ai_glmVec(aiVector3D aiVec)
{
	return { aiVec.x, aiVec.y, aiVec.z };
}


void Model::setShaderContext(Shader* shader)
{
	Model::shader = shader;
}

void Model::draw()
{
	for (Mesh& mesh : meshes) {
		for (GLint i = 0; i < textures.size(); i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			shader->setUniform(textures[i].uniform, i);
			glBindTexture(GL_TEXTURE_2D, textures[i].textureID);
		}

		mesh.vertexArray.bind();
		glDrawElements(GL_TRIANGLES, mesh.vertexArray.indices.size(), GL_UNSIGNED_INT, 0);
		mesh.vertexArray.unbind();
		glActiveTexture(GL_TEXTURE0);
	}
}

void Model::setTexturesFolder(std::string path)
{
	texturesFolder = path;
}
