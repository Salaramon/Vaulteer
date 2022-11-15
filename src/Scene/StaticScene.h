#pragma once

#include <glm/glm.hpp>

#include "Data Structures/BoundingSphereHierarchy.h"

#include "Scene/Scene.h"


template<class... SceneObjects>
class StaticScene : public Scene<BoundingSphereHierarchy, SceneObjects...>
{
public:
	using BaseScope = Scene<BoundingSphereHierarchy, SceneObjects...>;

	template<class StoreType>
	using StaticSceneIterator = BaseScope::template StoreRangeIterator<StoreType>;

	template<class StoreType>
	StoreType* addObject(StoreType&& object, glm::vec4 sphere) {
		DIRLOG::CTOR::debug(this, DY::std_format("Requested iterator range of type {} within {}", DY::types_to_string<StoreType>(), DY::glm_to_string(sphere)));
		return std::get<BaseScope::template ObjectContainer<StoreType>>(BaseScope::objectContainers).insert(std::make_unique<StoreType>(std::move(object)), sphere).get();
	}

	template<class StoreType>
	StoreType* addObject(StoreType& object, glm::vec4 sphere) {
		DIRLOG::CTOR::debug(this, DY::std_format("Requested iterator range of type {} within {}", DY::types_to_string<StoreType>(), DY::glm_to_string(sphere)));
		return std::get<BaseScope::template ObjectContainer<StoreType>>(BaseScope::objectContainers).insert(std::make_unique<StoreType>(object), sphere).get();
	}

	template<class StoreType>
	StaticSceneIterator<StoreType> get(std::function<bool(glm::vec4)> boundingFunction) {
		DIRLOG::CTOR::debug(this, DY::std_format("Requested iterator range of type {}", DY::types_to_string<StoreType>()));
		auto& container = std::get<BaseScope::template ObjectContainer<StoreType>>(BaseScope::objectContainers);
		return container.equal_range(boundingFunction);
	}

	//TODO: give error if not finialized before being used.
	void finalize() {
		DIRLOG::CTOR::debug(this, "Finalizing scene...");
		(std::get<typename BaseScope::template ObjectContainer<SceneObjects>>(BaseScope::objectContainers).establishHierarchy(), ...);
	}

	static inline auto OB = DY::ObjectBinder<decltype(BaseScope::OB)>();

	using LOG = _LOG<DY::No_CB, decltype(OB), DY::No_FB, DY::No_VB>;
};

