#pragma once

#include <type_traits>

template<class DedicatedRenderer>
class Renderer : public DedicatedRenderer
{
public:
	//template<class... Args>
	void render(DedicatedRenderer::Scene& staticScene, DedicatedRenderer::Scene& dynamicScene);
	void reload();
};

template<class DedicatedRenderer>
//template<class... Args>
inline void Renderer<DedicatedRenderer>::render(DedicatedRenderer::Scene& staticScene, DedicatedRenderer::Scene& dynamicScene) {
	DedicatedRenderer::render(staticScene, dynamicScene);
}

template<class DedicatedRenderer>
//template<class... Args>
inline void Renderer<DedicatedRenderer>::reload() {
	DedicatedRenderer::reloadShaders();
}
