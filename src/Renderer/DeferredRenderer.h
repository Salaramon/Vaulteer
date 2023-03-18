#pragma once

#include "Renderer/Techniques/DeferredGeometryTechnique.h"
#include "Renderer/Techniques/DeferredPointLightTechnique.h"
#include "Renderer/Techniques/DeferredDirLightTechnique.h"

#include "OpenGL.h"

#include "Renderer/BatchManager.h"
#include "Model/Model.h"
#include "API/Camera.h"
#include "Renderer/Buffers/GBuffer.h"
#include "Model/Resources/ResourcePack.h"


class DeferredRenderer : 
	public DeferredGeometryTechnique, 
	public DeferredPointLightTechnique {
private:

	inline static std::unique_ptr<GBuffer> gbuffer;
	inline static Mesh* quadMesh;
	inline static Mesh* sphereMesh;

	inline static GLint currentlyBoundTexture = -1;
	inline static bool buildBatch = true;

	// for debugging while we're not fetching lights from scene
	inline static bool buildLights = true;

	inline static BatchManager batchManager;

	inline static std::vector<PointLight> pointLights;

public:
	static void initialize(uint screenWidth, uint screenHeight);
	static void preload(ResourcePack& pack);
	static void reloadShaders();

	static void rebuildBatch();
	static void rebuildGBuffer(int width, int height);
	static void copyGBufferDepth(GLint fbo);

	static void geometryPass(const Camera* camera);
	static void directionalLightPass(const Camera* camera);
	static void lightingPass(const Camera* camera);
	static void singleLightVolumePass(const PointLight& light, const int index);

	template<size_t SCENE_ID>
	static void render(Scene<SCENE_ID>& scene) {
		/*
		auto cameraIteratorFirst = dynamicScene.template get<Camera>().first;
		auto* camera = (*cameraIteratorFirst).get();

		using s_model_iterator = DeferredStaticModelIteratorPair<StaticSceneObjects...>;
		const s_model_iterator& staticSceneIt = staticScene.template get<Opaque<Model<ModelData>>>([&](glm::vec4 ignored) -> bool {
			return true;
		});

		// TODO: 
		if (buildBatch) {
			for (auto it = staticSceneIt.first; it != staticSceneIt.second; it++) {
				auto& model = (*it)->model;
				BatchManager::setTextureID(batchManager, model.getData()->getTextureID());

				for (auto& mesh : model.getData()->getMeshes()) {
					BatchManager::addToBatch(batchManager, mesh, model.getModelMatrix());
				}
			}

			buildBatch = false;
		}
		
		OpenGL::enableDepthTest();
		geometryPass(camera);

		OpenGL::disableDepthTest();
		directionalLightPass(camera);

		OpenGL::enableCullFace(OpenGL::FRONT);

		OpenGL::enableBlending();
		OpenGL::setBlendMode(GLBlendModes::SourceAlpha, GLBlendModes::One);
		lightingPass(camera);
		OpenGL::disableBlending();
		OpenGL::disableCullFace();
		*/
	}

};
