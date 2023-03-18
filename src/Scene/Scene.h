#pragma once

#include <memory>
#include <tuple>
#include <type_traits>
#include <vector>
#include <tuple>

#include "entt.hpp"
#include "Entity.h"
#include "Register.h"
#include "Data Structures/BoundingSphereHierarchy.h"

#include "Debug/Debug.h"

#include <entt.hpp>



template<class... Args>
class ViewCollection {
public:
	ViewCollection(Args... views) : viewTuple(views... ) {}


	template<class Function>
	void each(Function& function) {
		(std::get<Args>(viewTuple).each(function), ...);
	}

private:
	std::tuple<Args...> viewTuple;
};





template<class T1, class T2>
class BSHView {};

template<class... Args1, class... Args2>
class BSHView<std::tuple<Args1...>, std::tuple<Args2...>> {
public:
	BSHView(const entt::registry& registry, const BoundingSphereHierarchy<Entity>& bsh, std::function<bool(glm::vec4)> comparator) : registry(registry), bsh(bsh), comparator(comparator) {}

	template<class Function>
	void each(Function& function) {
		auto [beginIt, endIt] = bsh.equal_range(comparator);
		for (auto& it = beginIt; it != bsh.end(); it++) {
			registry.group<Args1...>({}, { entt::exclude_t<Args2>... }).get(*it);
		}

	}

private:
	BoundingSphereHierarchy<Entity>::iterator beginIt;
	BoundingSphereHierarchy<Entity>::iterator endIt;

	entt::registry& registry;
	BoundingSphereHierarchy<Entity>& bsh;
	std::function<bool(glm::vec4)> comparator;
};

template<class T1, class T2>
class DefaultView {};

template<class... Args1, class... Args2>
class DefaultView<std::tuple<Args1...>, std::tuple<Args2...>> {
public:
	DefaultView(const entt::registry& registry) : registry(registry) {}

	template<class Function>
	void each(Function& function) {
		registry.view<Args1...>(entt::exclude_t<Args2>...);
	}

private:
	entt::registry& registry;
};


template<size_t SCENE_ID>
class Scene : public Register {
	template<size_t VALUE>
	struct SceneTag : std::integral_constant<size_t, VALUE> {};
	template<size_t VALUE>
	struct StoreTag : std::integral_constant<size_t, VALUE> {};


public:

	struct Flags {
		inline static constexpr size_t DEFAULT = 0b0001;
		inline static constexpr size_t BSH = 0b0010;
	};


	template<size_t FLAG = Flags::DEFAULT>
	void add(Entity& entity) {
		registry.emplace<SceneTag<SCENE_ID>>(entity);
		registry.emplace<StoreTag<FLAG>>(entity);
	}

	template<size_t FLAG = Flags::DEFAULT, auto Lambda>
	void add(Entity& entity) {
		registry.emplace<SceneTag<SCENE_ID>>(entity);
		registry.emplace<StoreTag<FLAG>>(entity);
		addToOptimizedStructure<FLAG, Lambda>(entity);
	}

	void remove(Entity entity) {
		registry.remove<SceneTag<SCENE_ID>>(entity);
	}

	template<size_t FROM_SCENE, size_t TO_SCENE>
	static void move(Entity entity, Scene<FROM_SCENE> from, Scene<TO_SCENE> to) {
		from.remove(entity);
		to.add(entity);
	}

	template<class Exclude>
	using ExcludeComponent = entt::exclude_t<Exclude>;


	template<class T>
	struct GetType {
	public:
		using Include = std::tuple<T>;
		using Exclude = std::tuple<>;
	};

	template<class T>
	struct GetType<ExcludeComponent<T>> {
	public:
		using Include = std::tuple<>;
		using Exclude = std::tuple<ExcludeComponent<T>>;
	};

	template<class... Args>
	struct ProcessComponentTypes {
	public:
		using IncludeTuple = std::_Tuple_cat1<typename GetType<Args>::Include...>;
		using ExcludeTuple = std::_Tuple_cat1<typename GetType<Args>::Exclude...>;
	};

	template<class Tuple1, class Tuple2>
	struct View {};

	template<class... Args1, class... Args2>
	struct View<std::tuple<Args1...>, std::tuple<Args2...>> {
	public:
		inline static auto view() {
			return ViewCollection(
				DefaultView<Args1..., Args2...>(registry)
			);
		}

		inline static auto view(std::function<bool(glm::vec4)> comparator) {
			return ViewCollection(
				DefaultView<Args1..., Args2...>(registry),
				BSHView<Args1..., Args2...>(registry, bsh, comparator)
			);
		}
	};

	template<class... Args>
	using InvokeView = View<typename ProcessComponentTypes<Args...>::IncludeTuple, typename ProcessComponentTypes<Args...>::ExcludeTuple>;

	template<class... Args>
	auto view() {
		return InvokeView::view();
	}

	template<class... Args>
	auto view(std::function<bool(glm::vec4)> comparator) {
		return InvokeView::view(comparator);
	}


	template<size_t FLAG, class... Types>
	std::tuple<Types...> get(Entity entity) {
		return removeTags(registry.get<SceneTag<SCENE_ID>, StoreTag<FLAG>, Types...>(entity.entity_id));
	}

	template<class... Types>
	std::tuple<Types...> get(Entity entity) {
		return get<Scene::Flags::DEFAULT, Types...>(entity);
	}

private:

	template<class Tag1, class Tag2, class... Rest>
	std::tuple<Rest...> removeTags(std::tuple<Tag1, Tag2, Rest...> tuple) {
		return std::make_tuple(std::get<Rest>(tuple)...);
	}

	template<auto LambdaFunction, class LambdaFunctionType = decltype(LambdaFunction)>
	struct Insert {};

	template<auto LambdaFunction, class ReturnType, class... Params>
	struct Insert<LambdaFunction, ReturnType(*)(Params...)> {
	public:
		static void insert(Entity& entity, BoundingSphereHierarchy<Entity>& bsh) {
			bsh.insert(entity, LambdaFunction(registry.get<Params>(entity)...));
		}
	};


	template<size_t FLAG, auto FUNC>
	void addToOptimizedStructure(const Entity& entity) {
		if constexpr (FLAG == Flags::BSH) {
			if(registry.try_get<StoreTag<FLAG>>(entity) != nullptr) {
				Insert<FUNC>::insert(entity, bsh);
			}
		}
	}

	BoundingSphereHierarchy<Entity> bsh;
};