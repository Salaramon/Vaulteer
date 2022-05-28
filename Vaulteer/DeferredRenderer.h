#include <type_traits>

#include "DeferredGeometryTechnique.h"
#include "DeferredLightingTechnique.h"
#include "DynamicScene.h"
#include "StaticScene.h"
#include "OpenGL.h"

#include "BatchManager.h"
#include "Model.h"
#include "Camera.h"
#include "GBuffer.h"
#include "ResourcePack.h"
#include "RendererPrerequisites.h"

using DeferredDynamicScene = DynamicScene<Camera>;
using DeferredStaticScene = StaticScene<Model<ModelData>>;

class DeferredRenderer : public RendererPrerequisites<DeferredDynamicScene, DeferredStaticScene>, public DeferredGeometryTechnique, public DeferredLightingTechnique {
private:
	using DeferredStaticSceneIteratorPair = typename DeferredStaticScene::template StaticSceneIterator<Model<ModelData>>;

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

	static void geometryPass(Camera* camera);
	static void lightingPass(Camera* camera);

	template<class... DynamicSceneObjects, class... StaticSceneObjects>
	static void render(DynamicScene<DynamicSceneObjects...>& dynamicScene, StaticScene<StaticSceneObjects...>& staticScene) {

		auto cameraIteratorFirst = dynamicScene.get<Camera>().first;
		auto* camera = (*cameraIteratorFirst).get();

		const DeferredStaticSceneIteratorPair& staticSceneIt = staticScene.get<Model<ModelData>>([&](glm::vec4 ignored) -> bool { return true; });

		if (buildBatch) {
			for (auto it = staticSceneIt.first; it != staticSceneIt.second; it++) {
				auto& model = *it;

				BatchManager::setTextureID(batchManager, model->getData()->getTextureID());

				for (auto& mesh : model->getData()->getMeshes()) {
					BatchManager::addToBatch(batchManager, mesh, model->getModelMatrix());
				}
			}

			buildBatch = false;
		}

		geometryPass(camera);
		lightingPass(camera);
	}
};