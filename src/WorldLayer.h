#pragma once
#include "API/Layer.h"
#include "API/CameraController.h"

#include "Renderer/Renderer.h"
#include "Renderer/BlendingForwardRenderer.h"
#include "Renderer/DeferredRenderer.h"


class WorldLayer : public Layer {
public:
	WorldLayer() : Layer("WorldLayer") {}
	~WorldLayer() override = default;

	void onAttach() override;

	void onUpdate(float timestep) override;
	void onEvent(BaseEvent& e) override;

private:
	CameraController cameraController;
	
	//Scenes
	inline static constexpr size_t scene_0 = 0;

	Scene<scene_0> scene;
	//StaticScene<Model<ModelData>> staticScene;
	//StaticScene<OpaqueModel, Model<LineData>> opaqueScene;
	//StaticScene<TransparentModel> transparentScene;
	
	//Renderer<DeferredRenderer> opaqueRenderer;
	//Renderer<BlendingForwardRenderer> transparentRenderer;
};

