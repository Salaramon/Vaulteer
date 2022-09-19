#pragma once
#include "API/Layer.h"
#include "API/CameraController.h"

#include "Renderer/Renderer.h"
#include "Renderer/BlendingForwardRenderer.h"
#include "Renderer/DeferredRenderer.h"


class WorldLayer : public Layer {
public:
	WorldLayer(std::string name) : Layer(name) {}

	void onAttach() override;
	void onUpdate(float timestep) override;

private:
	CameraController cameraController;
	
	//Scenes
	DynamicScene<Camera> dynamicScene;
	//StaticScene<Model<ModelData>> staticScene;
	StaticScene<OpaqueModel, Model<LineData>> opaqueScene;
	StaticScene<TransparentModel> transparentScene;
	
	Renderer<DeferredRenderer> opaqueRenderer;
	Renderer<BlendingForwardRenderer> transparentRenderer;
};

