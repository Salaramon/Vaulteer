#pragma once

#include <unordered_map>

#include "API/Application.h"

#include "Model/Model.h"
#include "Model/Resources/ResourceManager.h"

#include "Renderer/Renderer.h"
#include "Renderer/BlendingForwardRenderer.h"


class Vaulteer : public Application {
public:
	Vaulteer(const ApplicationSpecification& spec);

	static void loadResources();

	void setup() override;


};
