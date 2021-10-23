#pragma once
#include <vector>
#include <type_traits>
#include <typeinfo>
#include <string>

#include "RenderPass.h"
#include "DebugLogger.h"
#include "DebugAliases.h"

class Scene;

class RenderStrata : public DebugLogger<RenderStrata>
{
public:
	template<class T>
	RenderPass* addRenderPass(T&& pass);
	void render(SceneLayer& layer);
private:
	std::vector<std::unique_ptr<RenderPass>> renderPasses;
};

template<class T>
inline RenderPass* RenderStrata::addRenderPass(T&& pass)
{
	if (!std::is_base_of_v<RenderPass, T>) {
		debug(std::string(typeid(T).name()) + " " + "is not derived from " + std::string(typeid(RenderPass).name()), MessageAlias::CriticalError);
	}
	renderPasses.emplace_back(std::make_unique<T>(pass));
	return renderPasses.back().get();
}
