#include "ModelData.h"

ModelData::ModelData(GLsizei width, GLsizei height, std::vector<glm::vec4> colors, std::vector<Vertex> vertices)
{
	std::vector<Vertex> vert;
	std::vector<GLuint> indi;
	storePointsAndIndices<ModelVertexHash>(indi, vert, vertices);
	textures.emplace_back(Binder::forward_frag::uniforms::diffuse1, width, height, colors);
	//Implement a set color in texture, and make texture take an abstract amount of pixel data to set.
	//Take argument in this function for binder uniform, for the call above.
	meshes.emplace_back(vert, indi);
}

ModelData::ModelData(std::string meshPath)
{
	debug("Loading model: " + meshPath + "\n");
	loadModel(meshPath);
}

ModelData::ModelData(std::string meshPath, std::string textureFolderPath)
{
	setTexturesFolder(textureFolderPath);
	loadModel(meshPath);
}


void ModelData::loadModel(std::string path)
{

	Assimp::Importer modelImporter;
	const aiScene* scene = modelImporter.ReadFile(path, aiProcess_GenSmoothNormals | aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		debug("Assimp Error: " + std::string(modelImporter.GetErrorString()) + "\n", MessageAlias::CriticalError);
	}

	if (scene) {
		processNode(scene, scene->mRootNode);
	}
}

void ModelData::processNode(const aiScene* scene, aiNode* node)
{
	for (size_t i = 0; i < node->mNumMeshes; i++) {
		aiMesh* aiMesh = scene->mMeshes[node->mMeshes[i]];

		meshes.emplace_back(std::move(processMesh(scene, aiMesh)));
	}

	for (size_t i = 0; i < node->mNumChildren; i++) {
		processNode(scene, node->mChildren[i]);
	}
}

Mesh ModelData::processMesh(const aiScene* scene, aiMesh* mesh)
{
	Vertices vertices;
	Indices indices;
	std::vector<Texture> textures;

	for (size_t i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		vertex.aPos = ai_glmVec(mesh->mVertices[i]);

		if (mesh->HasNormals()) {
			vertex.aNormal = ai_glmVec(mesh->mNormals[i]);
		}
		else {
			vertex.aNormal = glm::vec3(0);
		}

		if (mesh->mTextureCoords[0]) {
			vertex.aTexCoords = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
		}
		else {
			vertex.aTexCoords = glm::vec2(0.0f, 0.0f);
		}

		// random
		//vertex.shininess = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		//vertex.c = glm::fvec3(rand() % 2, rand() % 2, rand() % 2);

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

	/*
	std::vector<std::array<float, 3>> inVert;
	for (size_t i = 0; i < vertices.size(); i++) {
		inVert.push_back(*((std::array<float,3>*)(&vertices[i].aPos)));
	}
	std::vector<std::array<float, 3>> outVert;
	std::vector<size_t> outInd;
	processConvexShape<3,3>(inVert, outVert, outInd);
	*/

	return Mesh(vertices, indices);
}

void ModelData::getTextureUniforms(aiMaterial* material)
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

glm::vec3 ModelData::ai_glmVec(aiVector3D aiVec)
{
	return { aiVec.x, aiVec.y, aiVec.z };
}

void ModelData::setTexturesFolder(std::string path)
{
	texturesFolder = path;
}

glm::vec4 ModelData::getBoundingSphere()
{
	std::vector<Seb::Point<double>> points;
	for (const Mesh& m : meshes) {
		for (const Vertex& v : m.vertices) {
			std::vector<double> converter({ v.aPos.x, v.aPos.y, v.aPos.z });
			points.push_back(Seb::Point<double>(3, converter.begin()));
		}
	}
	Seb::Smallest_enclosing_ball<double> ball(3, points);
	Seb::Smallest_enclosing_ball<double>::Coordinate_iterator it = ball.center_begin();
	
	glm::vec4 data(it[0], it[1], it[2], ball.radius());

	return data;
}


void ModelData::draw(const Shader& shader)
{

	for (Mesh& mesh : meshes) {
		for (GLint i = 0; i < textures.size(); i++) {
			textures[i].activate(shader, i);
		}

		mesh.vertexArray.bind();
		//glDrawElementsInstanced(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0, instances.size());
		debug("Drawing mesh: " + std::to_string(mesh.getObjectKey()) + "\n", "glDrawElements");
		glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
		mesh.vertexArray.unbind();

		//Is setting active texture back to 0 unnecessary?
		glActiveTexture(GL_TEXTURE0);
	}
}

const std::vector<Mesh>& ModelData::getMeshes()
{
	return meshes;
}

const std::vector<Texture>& ModelData::getTextures()
{
	return textures;
}
