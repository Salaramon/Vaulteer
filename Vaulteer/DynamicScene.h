#pragma once

#include <memory>
#include <tuple>
#include <type_traits>
#include <vector>

#include "Scene.h"

template<class... SceneObjects>
class DynamicScene : public Scene<std::vector, SceneObjects...>
{
public:
	using BaseScope = Scene<std::vector, SceneObjects...>;

	template<class StoreType>
	StoreType* addObject(StoreType&& object) {
		std::get<BaseScope::template ObjectContainer<StoreType>>(BaseScope::objectContainers).emplace_back(std::make_unique<StoreType>(std::move(object)));
		return std::get<BaseScope::template ObjectContainer<StoreType>>(BaseScope::objectContainers).back().get();
	}

	template<class StoreType>
	StoreType* addObject(StoreType& object) {
		std::get<BaseScope::template ObjectContainer<StoreType>>(BaseScope::objectContainers).emplace_back(std::make_unique<StoreType>(object));
		return std::get<BaseScope::template ObjectContainer<StoreType>>(BaseScope::objectContainers).back().get();
	}

	template<class StoreType>
	const std::pair<typename BaseScope::template iterator<StoreType>, typename BaseScope::template iterator<StoreType>> get() {
		using it = typename BaseScope::template iterator<StoreType>;
		auto& container = std::get<BaseScope::template ObjectContainer<StoreType>>(BaseScope::objectContainers);
		return std::make_pair(container.begin(), container.end());
	}
};