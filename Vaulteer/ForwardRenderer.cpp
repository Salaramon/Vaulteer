#include "vpch.h"
#include "ForwardRenderer.h"


void ForwardRenderer::render(Scene& scene) {
	ForwardTechnique::shader->use();
	const SceneObjects<typename Model<ModelData>>& modelDataVector = scene.getVector<Model<ModelData>>();
	const SceneObjects<typename Camera>& cameraDataVector = scene.getVector<Camera>();

	ForwardTechnique::setProjection(cameraDataVector.front()->getProjectionMatrix());

	glm::mat4 viewMatrix = cameraDataVector.front()->getViewMatrix();
	ForwardTechnique::setInverseViewMatrix(viewMatrix);

	ForwardTechnique::shader->setUniform(Binder::forward_frag::uniforms::cameraPos, 1, cameraDataVector.front()->getPosition());
	ForwardTechnique::shader->setUniform(Binder::forward_frag::uniforms::lightPos, 1, cameraDataVector.front()->getPosition());

	for (auto& data : modelDataVector) {
		ForwardTechnique::setModelView(data->getModelMatrix(), viewMatrix);

		ModelData* modelData = data->getData();
		const std::vector<Mesh>& modelDataMeshes = modelData->getMeshes();

		for (const Mesh& mesh : modelDataMeshes) {
			GLint texUnit = 0;

			ForwardTechnique::shader->setUniform(Binder::forward_frag::uniforms::textureLib, texUnit);
			//glBindTextureUnit(texUnit, modelDataTextureArray.getTextureID());

			//mesh.vertexArray.bind();
		
			//glDrawElementsInstanced(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0, instances.size());
			//debug("Drawing mesh: " + std::to_string(mesh.getObjectKey()) + "\n", "glDrawElements");
			glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);

			//mesh.vertexArray.unbind();

			texUnit++;
		}
	}

	/*LineTechnique::shader->use();
	const SceneObjects<typename Model<LineData>>& lineDataVector = scene.getVector<Model<LineData>>();

	LineTechnique::setProjection(cameraDataVector.front()->getProjectionMatrix());
	LineTechnique::setView(cameraDataVector.front()->getViewMatrix());
	for (auto& data : lineDataVector) {
		LineTechnique::setModel(data->getModelMatrix());
		LineData* lineData = data->getData();
		LineTechnique::setColor(lineData->color);

		lineData->vertexArray.bind();
		glDrawElements(GL_LINES, lineData->indices.size(), GL_UNSIGNED_INT, 0);
		lineData->vertexArray.unbind();
		//Is setting active texture back to 0 unnecessary?
		glActiveTexture(GL_TEXTURE0);
	}*/
}

