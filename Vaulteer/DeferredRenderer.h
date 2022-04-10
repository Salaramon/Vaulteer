#include <type_traits>

#include "DeferredGeometryTechnique.h"
#include "DeferredLightingTechnique.h"
#include "Scene.h"

#include "Model.h"
#include "ModelData.h"
#include "LineData.h"
#include "Camera.h"
#include "GBuffer.h"
#include "ResourcePack.h"

class DeferredRenderer : public DeferredGeometryTechnique, public DeferredLightingTechnique
{
public:
	using Scene = Scene<Camera, Model<ModelData>>;

	void initialize(uint screenWidth, uint screenHeight);
	void preload(ResourcePack& pack);

	void render(Scene& scene);

	void geometryPass(const SceneObjects<typename Model<ModelData>>& modelDataVector, const SceneObjects<typename Camera>& cameraDataVector);
	void lightingPass(const SceneObjects<typename Model<ModelData>>& modelDataVector, const SceneObjects<typename Camera>& cameraDataVector);

	void reloadShaders();

private:
	std::unique_ptr<GBuffer> gbuffer;
	std::unique_ptr<ModelData> quad;
};