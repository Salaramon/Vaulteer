#pragma once

#include <type_traits>

template<class DedicatedRenderer>
class Renderer : public DedicatedRenderer
{
public:
	//template<class... Args>
	void render(DedicatedRenderer::Scene& scene);
};

template<class DedicatedRenderer>
//template<class... Args>
inline void Renderer<DedicatedRenderer>::render(DedicatedRenderer::Scene& scene)
{
	DedicatedRenderer::render(scene);
}
