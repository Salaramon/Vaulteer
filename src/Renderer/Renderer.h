#pragma once

#include <type_traits>
#include <tuple>
#include <utility>

#include "Scene/Scene.h"



template<class... Renderers>
class Renderer {
public:
	template<size_t SCENE_ID>
	void render(Scene<SCENE_ID>& scene) {
		(Renderers::render(scene), ...);
	}
	
};
