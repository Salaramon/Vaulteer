#include "ForwardRenderer.h"


void ForwardRenderer::render(Scene& scene)
{
	ForwardTechnique::shader->use();
	const SceneObjects<typename Model<ModelData>>& modelDataVector = scene.getVector<Model<ModelData>>();
	const SceneObjects<typename Camera>& cameraDataVector = scene.getVector<Camera>();

	ForwardTechnique::setProjection(cameraDataVector.front()->getProjectionMatrix());
	ForwardTechnique::setView(cameraDataVector.front()->getViewMatrix());
	for (auto& data : modelDataVector) {
		ForwardTechnique::setModel(data->getModelMatrix());

		ModelData* modelData = data->getData();
		const std::vector<Mesh>& modelDataMeshes = modelData->getMeshes();
		const std::vector<Texture>& modelDataTextures = modelData->getTextures();

		for (const Mesh& mesh : modelDataMeshes) {
			for (GLint i = 0; i < modelDataTextures.size(); i++) {
				modelDataTextures[i].activate(*ForwardTechnique::shader , i);
			}

			mesh.vertexArray.bind();
			//glDrawElementsInstanced(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0, instances.size());
			//debug("Drawing mesh: " + std::to_string(mesh.getObjectKey()) + "\n", "glDrawElements");
			glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
			mesh.vertexArray.unbind();

			//Is setting active texture back to 0 unnecessary?
			glActiveTexture(GL_TEXTURE0);
		}
	}

	LineTechnique::shader->use();
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
	}
}

