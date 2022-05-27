#include <type_traits>

#include "DeferredGeometryTechnique.h"
#include "DeferredLightingTechnique.h"
#include "Scene.h"
#include "OpenGL.h"

#include "BatchManager.h"
#include "Model.h"
#include "Camera.h"
#include "GBuffer.h"
#include "ResourcePack.h"

class DeferredRenderer : public DeferredGeometryTechnique, public DeferredLightingTechnique
{
public:
	using Scene = Scene<Camera, Model<ModelData>>;

	void initialize(uint screenWidth, uint screenHeight);
	void preload(ResourcePack& pack);

	//void render(Scene& scene); we'll fix this later
	void render(Scene& staticScene, Scene& dynamicScene);

	void geometryPass(const SceneObjects<typename Model<ModelData>>& modelDataVector, Camera* camera);
	void lightingPass(const SceneObjects<typename Model<ModelData>>& modelDataVector, Camera* camera);

	void reloadShaders();
	void rebuildBatch();

private:
	std::unique_ptr<GBuffer> gbuffer;
	std::unique_ptr<ModelData> quad;

	GLint currentlyBoundTexture = -1;

	BatchManager batchManager;
	bool buildBatch = true;
};