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

	static void initShaders();
	static void loadResources();

	void setup() override;

private:
	template <class T>
	Model<T> modelByName(std::unordered_map<std::string, std::unique_ptr<T>>& container, std::string name);

	std::unordered_map<std::string, std::unique_ptr<ModelData>> models;
	std::unordered_map<std::string, std::unique_ptr<LineData>> lines;
};

template <class T>
Model<T> Vaulteer::modelByName(std::unordered_map<std::string, std::unique_ptr<T>>& container, std::string name) {
	auto it = container.find(name);
	assert(it != container.end(), "Model name not found.");

	return Model<T>(*it->second);
}
