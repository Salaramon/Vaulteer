#include "Renderer/Techniques/DeferredGeometryTechnique.h"
#include "Renderer/Techniques/DeferredLightingTechnique.h"
#include "Scene/DynamicScene.h"
#include "Scene/StaticScene.h"
#include "OpenGL.h"

#include "Renderer/BatchManager.h"
#include "Model/Model.h"
#include "API/Camera.h"
#include "Renderer/Buffers/GBuffer.h"
#include "Model/Resources/ResourcePack.h"
#include "Renderer/RendererPrerequisites.h"
#include "Renderer/Tags/Opaque.h"

using deferred_dynamic_scene = DynamicScene<Camera>;
using deferred_static_scene = StaticScene<Opaque<Model<ModelData>>>;

class DeferredRenderer : public RendererPrerequisites<deferred_dynamic_scene, deferred_static_scene>, public DeferredGeometryTechnique, public DeferredLightingTechnique {
private:
	template <class... Args>
	using DeferredStaticModelIteratorPair = typename StaticScene<Args...>::template StaticSceneIterator<Opaque<Model<ModelData>>>;

	inline static std::unique_ptr<GBuffer> gbuffer;
	inline static std::unique_ptr<ModelData> quad;

	inline static GLint currentlyBoundTexture = -1;
	inline static bool buildBatch = true;

	inline static BatchManager batchManager;

public:
	static void initialize(uint screenWidth, uint screenHeight);
	static void preload(ResourcePack& pack);
	static void reloadShaders();

	static void rebuildBatch();
	static void rebuildGBuffer(int width, int height);
	static void copyGBufferDepth(GLint fbo);

	static void geometryPass(const Camera* camera);
	static void lightingPass(const Camera* camera);

	template <class... DynamicSceneObjects, class... StaticSceneObjects>
	static void render(DynamicScene<DynamicSceneObjects...>& dynamicScene, StaticScene<StaticSceneObjects...>& staticScene) {

		auto cameraIteratorFirst = dynamicScene.template get<Camera>().first;
		auto* camera = (*cameraIteratorFirst).get();

		using s_model_iterator = DeferredStaticModelIteratorPair<StaticSceneObjects...>;
		const s_model_iterator& staticSceneIt = staticScene.template get<Opaque<Model<ModelData>>>([&](glm::vec4 ignored) -> bool {
			return true;
		});

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

		geometryPass(camera);
		lightingPass(camera);
	}
};
