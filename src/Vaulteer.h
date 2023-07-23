#pragma once

#include <unordered_map>

#include "API/Application.h"

#include "Model/Model.h"
#include "Model/Resources/ResourceManager.h"

#include "Renderer/Renderer.h"
#include "Renderer/BlendingForwardRenderer.h"


class Vaulteer : public Application {
public:
	std::vector<std::vector<Mesh*>> internalMeshes;

	Vaulteer(const ApplicationSpecification& spec);

	void loadResources();

	void setup() override;


};
