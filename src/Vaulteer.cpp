#include "vpch.h"
#include "Vaulteer.h"

#include "WorldLayer.h"
#include "DebugLayer.h"

Vaulteer::Vaulteer(const ApplicationSpecification& spec) : Application(spec) {
	initShaders();
	loadResources();
}

void Vaulteer::initShaders() {
	ShaderProgram<BlendingShader>::loadShader();
	ShaderProgram<BlendingCompositeShader>::loadShader();
	ShaderProgram<LineShader>::loadShader();
	ShaderProgram<DeferredDirShader>::loadShader();
	ShaderProgram<DeferredPointShader>::loadShader();
	ShaderProgram<GeometryShader>::loadShader();
	ShaderProgram<VolumeShader>::loadShader();
}

void Vaulteer::loadResources() {
	std::vector<ModelResourceLocator> locators = {
		{ "crate", "resources/crate/crate1.obj" },
		{ "palm", "resources/palm/palm.obj", aiProcess_GenNormals | aiProcess_Triangulate | aiProcess_FlipUVs },
		//{ "plane", "resources/plane/plane_mesh.obj", aiProcess_GenNormals | aiProcess_Triangulate },
		//{ "vase", "resources/vase/untitled.obj" },
		//{ "backpack", "backpack/backpack.obj" },
		//{ "teapot", "resources/backpack/teapot.obj", aiProcess_GenSmoothNormals | aiProcess_Triangulate | aiProcess_FlipUVs },
	};

	ResourceManager::createPack(locators);
}

void Vaulteer::setup() {
	//glEnable(GL_LINE_SMOOTH);
	glfwSetInputMode(window->getRawWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	glClearColor(0.00f, 0.00f, 0.00f, 1.0f);

	layerStack.pushLayer(new WorldLayer());
	layerStack.pushLayer(new DebugLayer());
}
