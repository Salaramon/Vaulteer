#pragma once

#include <glm/glm.hpp>

#include "BoundingSphereHierarchy.h"

#include "Scene.h"


template<class... SceneObjects>
class StaticScene : public Scene<BoundingSphereHierarchy, SceneObjects...>
{
	using BaseScope = Scene<BoundingSphereHierarchy, SceneObjects...>;
public:

	template<class StoreType>
	StoreType* addObject(StoreType&& object, glm::vec4 sphere) {
		return std::get<BaseScope::template ObjectContainer<StoreType>>(BaseScope::objectContainers).insert(std::make_unique<StoreType>(std::move(object)), sphere).get();
	}

	template<class StoreType>
	StoreType* addObject(StoreType& object, glm::vec4 sphere) {
		return std::get<BaseScope::template ObjectContainer<StoreType>>(BaseScope::objectContainers).insert(std::make_unique<StoreType>(object), sphere).get();
	}

	template<class StoreType>
	const std::pair<typename BaseScope::template iterator<StoreType>, typename BaseScope::template iterator<StoreType>> get(std::function<bool(glm::vec4)> boundingFunction) {
		auto& container = std::get<BaseScope::template ObjectContainer<StoreType>>(BaseScope::objectContainers);
		return container.equal_range(boundingFunction);
	}

	//TODO: give error if not finialized before being used.
	void finalize() {
		(std::get<typename BaseScope::template ObjectContainer<SceneObjects>>(BaseScope::objectContainers).establishHierarchy(), ...);
	}

};

