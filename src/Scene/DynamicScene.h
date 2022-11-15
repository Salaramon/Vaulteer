#pragma once

#include <memory>
#include <tuple>
#include <type_traits>
#include <vector>

#include "Scene/Scene.h"

#include "Debug/Debug.h"

template<class... SceneObjects>
class DynamicScene : public Scene<std::vector, SceneObjects...>
{
public:
	using BaseScope = Scene<std::vector, SceneObjects...>;

	template<class StoreType>
	using DynamicSceneIterator = BaseScope::template StoreRangeIterator<StoreType>;

	template<class StoreType>
	StoreType* addObject(StoreType&& object) {
		DIRLOG::CTOR::debug(this, DY::std_format("Requested iterator range of type {}", DY::types_to_string<StoreType>()));
		std::get<BaseScope::template ObjectContainer<StoreType>>(BaseScope::objectContainers).emplace_back(std::make_unique<StoreType>(std::move(object)));
		return std::get<BaseScope::template ObjectContainer<StoreType>>(BaseScope::objectContainers).back().get();
	}

	template<class StoreType>
	StoreType* addObject(StoreType& object) {
		DIRLOG::CTOR::debug(this, DY::std_format("Requested iterator range of type {}", DY::types_to_string<StoreType>()));
		std::get<BaseScope::template ObjectContainer<StoreType>>(BaseScope::objectContainers).emplace_back(std::make_unique<StoreType>(object));
		return std::get<BaseScope::template ObjectContainer<StoreType>>(BaseScope::objectContainers).back().get();
	}

	template<class StoreType>
	const DynamicSceneIterator<StoreType> get() {
		//LOG::CLAS::debug<&DynamicScene<SceneObjects...>::get<StoreType>>(this, &DynamicScene::objectContainers, DY::std_format("Requested iterator range of type {}", DY::types_to_string<StoreType>()));
		DIRLOG::CTOR::debug(this, DY::std_format("Requested iterator range of type {}", DY::types_to_string<StoreType>()));

		using it = typename BaseScope::template iterator<StoreType>;
		auto& container = std::get<BaseScope::template ObjectContainer<StoreType>>(BaseScope::objectContainers);
		return std::make_pair(container.begin(), container.end());
	}

	static inline auto OB = DY::ObjectBinder<decltype(BaseScope::OB)>();

	using LOG = _LOG<DY::No_CB, decltype(OB), DY::No_FB, DY::No_VB>;
};