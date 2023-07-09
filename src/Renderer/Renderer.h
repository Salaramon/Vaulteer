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

	void reloadShaders() {
		(Renderers::loadShaders(), ...);
	}


	// Statistics

	template<class... R>
	size_t sum(R... r) {
	  return (r + ...);
	}

	size_t getNumDrawCalls() {
		return sum(Renderers::stats.drawCalls...);
	}

	void resetStats() {
		(Renderers::resetStats(), ...);
	}
};
