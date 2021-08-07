#include "Model.h"

Model::Model(std::string meshPath)
{
	debug("Loading model: " + meshPath + "\n");
	loadModel(meshPath);
}

Model::Model(std::string meshPath, std::string textureFolderPath, const size_t nrInstances) :
	modelRotation(nrInstances, glm::mat4(1.0f)),
	modelScale(nrInstances, glm::mat4(1.0f)),
	modelTranslation(nrInstances, glm::mat4(1.0f)),
	instances(nrInstances, glm::mat4(1.0f)),
	instanceBuffer(instances)
{
	setTexturesFolder(textureFolderPath);
	loadModel(meshPath);
}

//OBJECT
Model& Model::operator[](size_t instance)
{
	instanceSelection = instance;
	return *this;
}

void Model::loadModel(std::string path)
{

	Assimp::Importer modelImporter;
	const aiScene* scene = modelImporter.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		debug("Assimp Error: " + std::string(modelImporter.GetErrorString()) + "\n", MessageAlias::CriticalError);
	}

	if (scene) {
		processNode(scene, scene->mRootNode);
	}
}

void Model::processNode(const aiScene* scene, aiNode* node)
{
	for (size_t i = 0; i < node->mNumMeshes; i++) {
		aiMesh* aiMesh = scene->mMeshes[node->mMeshes[i]];

		meshes.emplace_back(std::move(processMesh(scene, aiMesh)));
	}

	for (size_t i = 0; i < node->mNumChildren; i++) {
		processNode(scene, node->mChildren[i]);
	}
}

Mesh Model::processMesh(const aiScene* scene, aiMesh* mesh)
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



	return Mesh(vertices, indices, instanceBuffer);
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
					debug("Uniform for texture type " + std::to_string(type) + " not available.\n\tSee assimp's aiTextureTypes.\n");
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

void Model::rotate(float angle, glm::vec3 axis)
{
	modelRotation[instanceSelection] = glm::rotate(modelRotation[instanceSelection], angle, axis);
	instanceSelection = 0;
}

void Model::setRotation(float angle, glm::vec3 axis)
{
	modelRotation[instanceSelection] = glm::rotate(glm::mat4(1.0f), angle, axis);
	instanceSelection = 0;
}

void Model::move(float x, float y, float z)
{
	modelTranslation[instanceSelection] = glm::translate(modelTranslation[instanceSelection], glm::vec3(x, y, z));
	instanceSelection = 0;
}

void Model::setPosition(float x, float y, float z)
{
	modelTranslation[instanceSelection] = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
	instanceSelection = 0;
}

void Model::scale(float x, float y, float z)
{
	modelScale[instanceSelection] = glm::scale(modelScale[instanceSelection], glm::vec3(x, y, z));
	instanceSelection = 0;
}

void Model::setScale(float x, float y, float z)
{
	modelScale[instanceSelection] = glm::scale(glm::mat4(1.0f), glm::vec3(x, y, z));
	instanceSelection = 0;
}

void Model::draw()
{
	//shader->setUniform(Binder::vertex::uniforms::model, 1, GL_FALSE, modelMatrix);

	for (size_t i = 0; i < instances.size(); i++) {
		instances[i] = modelTranslation[i] * modelScale[i] * modelRotation[i];
	}
	//CAN BE OPTIMIZED TO ONLY REPLACE THE DATA THAT WAS CHANGED IN THE ARRAY
	instanceBuffer.replace(0, instances);

	for (Mesh& mesh : meshes) {
		for (GLint i = 0; i < textures.size(); i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			shader->setUniform(textures[i].uniform, i);
			glBindTexture(GL_TEXTURE_2D, textures[i].textureID);
		}

		mesh.vertexArray.bind();
		glDrawElementsInstanced(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0, instances.size());
		debug("Drawing mesh: " + std::to_string(mesh.getObjectKey()) + "\n", "glDrawElementsInstanced");
		//glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
		mesh.vertexArray.unbind();
		glActiveTexture(GL_TEXTURE0);
	}
}

void Model::setTexturesFolder(std::string path)
{
	texturesFolder = path;
}
