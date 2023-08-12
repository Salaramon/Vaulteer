#include "vpch.h"
#include "Vaulteer.h"

#include "WorldLayer.h"
#include "DebugLayer.h"

Vaulteer::Vaulteer(const ApplicationSpecification& spec) : Application(spec) {}

void Vaulteer::setup() {
	loadResources();

	glfwSetInputMode(Window::getRawWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	layerStack.pushLayer(&WorldLayer::worldFunctions);
	layerStack.pushLayer(&DebugLayer::debugFunctions);
}

void Vaulteer::loadResources() {
	// internal resources
	internalMeshes.push_back(ResourceLoader::importModel("resources/quad.obj", true));
	internalMeshes.push_back(ResourceLoader::importModel("resources/sphere-hd.obj", false));
	internalMeshes.push_back(ResourceLoader::importModel("resources/cone.obj", false));

	std::vector<ModelResourceLocator> locators = {
		{ "crate", "resources/crate/crate1.obj" },
		{ "palm", "resources/palm/palm.obj" },
		{ "quad", "resources/quad.obj" },
		//{ "plane", "resources/plane/plane_mesh.obj", aiProcess_GenNormals | aiProcess_Triangulate },
		//{ "backpack", "resources/backpack/backpack.obj" },
		//{ "teapot", "resources/backpack/teapot.obj", aiProcess_GenSmoothNormals | aiProcess_Triangulate | aiProcess_FlipUVs },
	};

	ResourceManager::createPack(locators);
}
