#include "vpch.h"
#include "Vaulteer.h"

#include "WorldLayer.h"
#include "DebugLayer.h"

Vaulteer::Vaulteer(const ApplicationSpecification& spec) : Application(spec) {}

void Vaulteer::setup() {
	loadResources();

	//glEnable(GL_LINE_SMOOTH);
	glfwSetInputMode(Window::getRawWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	auto world = new WorldLayer();
	layerStack.pushLayer(world);
	layerStack.pushLayer(new DebugLayer(world));
}

void Vaulteer::loadResources() {
	std::vector<ModelResourceLocator> locators = {
		{ "crate", "resources/crate/crate1.obj" },
		{ "cratei", "resources/crate/crate2.obj" },
		{ "palm", "resources/palm/palm.obj", aiProcess_GenNormals | aiProcess_Triangulate | aiProcess_FlipUVs },
		{ "quad", "resources/quad.obj" },
		//{ "plane", "resources/plane/plane_mesh.obj", aiProcess_GenNormals | aiProcess_Triangulate },
		//{ "backpack", "resources/backpack/backpack.obj" },
		//{ "teapot", "resources/backpack/teapot.obj", aiProcess_GenSmoothNormals | aiProcess_Triangulate | aiProcess_FlipUVs },
	};

	ResourceManager::createPack(locators);
}
