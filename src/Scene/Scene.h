#pragma once

#include <memory>
#include <tuple>
#include <type_traits>
#include <vector>
#include <tuple>
#include <functional>

#include "entt.hpp"
#include "Data Structures/BoundingSphereHierarchy.h"

#include "API/Camera.h"


#include <entt.hpp>



template<class... Args>
class ViewCollection {
public:
	ViewCollection(Args... views) : viewTuple(views... ) {}


	template<class Function>
	void each(Function function) {
		(std::get<Args>(viewTuple).each(std::forward<Function>(function)), ...);
	}

private:
	std::tuple<Args...> viewTuple;
};

template<class Tag, class... Rest>
inline std::tuple<Rest...> removeTags1(std::tuple<Tag, Rest...> tuple) {
	return std::tuple<Rest...>(std::forward<Rest>(std::get<Rest>(tuple))...);
}

template<class Tag1, class Tag2, class... Rest>
inline std::tuple<Rest...> removeTags2(std::tuple<Tag1, Tag2, Rest...> tuple) {
	return std::tuple<Rest...>(std::forward<Rest>(std::get<Rest>(tuple))...);
}

template<size_t VALUE>
struct SceneTag : std::integral_constant<size_t, VALUE> {
public:
	static constexpr auto page_size = ENTT_PACKED_PAGE;
};
template<size_t VALUE>
struct StoreTag : std::integral_constant<size_t, VALUE> {
	static constexpr auto page_size = ENTT_PACKED_PAGE;
};




template<class T1, class T2>
class BSHView {};

template<class... Args1, class... Args2>
class BSHView<std::tuple<Args1...>, std::tuple<entt::exclude_t<Args2>...>> {
public:
	BSHView(const entt::registry& registry, const BoundingSphereHierarchy<Entity>& bsh, std::function<bool(glm::vec4)> comparator) : registry(registry), bsh(bsh), comparator(comparator) {}

	template<class Function>
	void each(Function& function) {
		auto [beginIt, endIt] = bsh.equal_range(comparator);
		for (auto& it = beginIt; it != bsh.end(comparator); it++) {
			auto componentTuple = registry.group<std::tuple<Args1...>>({}, { entt::exclude_t<Args2>()... }).get(*it);
			auto trimmedComponentTuple = removeTags2(componentTuple);
			std::apply(function, trimmedComponentTuple);
		}

	}

private:
	BoundingSphereHierarchy<Entity>::iterator beginIt;
	BoundingSphereHierarchy<Entity>::iterator endIt;

	entt::registry& registry;
	BoundingSphereHierarchy<Entity>& bsh;
	std::function<bool(glm::vec4)> comparator;
};

template<class Function>
static void invokeLambda(Function function) {

}

template<class T1, class T2>
class DefaultView {};

template<class T1, class T2, class... Args1, class... Args2>
class DefaultView<std::tuple<T1, T2, Args1...>, std::tuple<entt::exclude_t<Args2>...>> {
public:
	DefaultView(const entt::basic_registry<uint64_t>& registry) : registry(registry) {}

	template<class Function>
	void each(Function function) {
		auto view = registry.view<Args1...>(entt::exclude_t<Args2>()...);
		for (auto tuple : view.each()) {
			auto trimmedComponentTuple = removeTags1(tuple);
			function(std::get<const Args1&>(trimmedComponentTuple)...);
			//std::invoke(std::forward<Function>(function), trimmedComponentTuple);
		}
		//view.each([function](T1, T2, Args1... args) { function(args...); });
	}

private:
	const entt::basic_registry<uint64_t>& registry;
};


template<class Exclude>
using ExcludeComponent = entt::exclude_t<Exclude>;


template<size_t SCENE_ID>
class Scene : public Register {
public:

	struct Flags {
		inline static constexpr size_t DEFAULT = 0b0001;
		inline static constexpr size_t BSH = 0b0010;
	};

	Camera* activeCamera = nullptr;

	CameraReference getActiveCamera() {
		return std::move(CameraReference(*activeCamera));
	}


	template<size_t FLAG = Flags::DEFAULT>
	void add(Entity& entity) {
		registry.emplace<SceneTag<SCENE_ID>>(entity.entity_id);
		registry.emplace<StoreTag<FLAG>>(entity.entity_id);
	}

	template<size_t FLAG = Flags::DEFAULT, auto Lambda>
	void add(Entity& entity) {
		registry.emplace<SceneTag<SCENE_ID>>(entity.entity_id);
		registry.emplace<StoreTag<FLAG>>(entity.entity_id);
		addToOptimizedStructure<FLAG, Lambda>(entity.entity_id);
	}



	void remove(Entity entity) {
		registry.remove<SceneTag<SCENE_ID>>(entity);
	}

	template<size_t FROM_SCENE, size_t TO_SCENE>
	static void move(Entity entity, Scene<FROM_SCENE> from, Scene<TO_SCENE> to) {
		from.remove(entity);
		to.add(entity);
	}



	template<class T1, class T2>
	struct _1_ConcatenateTuples {};

	template<class... Args1, class... Args2>
	struct _1_ConcatenateTuples<std::tuple<Args1...>, std::tuple<Args2...>> {
	public:
		using type = std::tuple<Args1..., Args2...>;
	};

	template<class T, class... Args>
	struct _0_ConcatenateTuples {
		using type = _1_ConcatenateTuples<T, typename _0_ConcatenateTuples<Args...>::type>::type;
	};

	template<class T>
	struct _0_ConcatenateTuples<T> {
	public:
		using type = T;
	};

	template<class... Args>
	using ConcatenateTuples = _0_ConcatenateTuples<Args...>::type;



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
		using IncludeTuple = ConcatenateTuples<typename GetType<Args>::Include...>;
		using ExcludeTuple = ConcatenateTuples<typename GetType<Args>::Exclude...>;
	};

	template<class Tuple1, class Tuple2>
	struct View {};

	template<class... Args1, class... Args2>
	struct View<std::tuple<Args1...>, std::tuple<Args2...>> {
	public:
		inline static auto view() {
			return ViewCollection(
				DefaultView<std::tuple<SceneTag<SCENE_ID>, StoreTag<Flags::DEFAULT>, Args1...>, std::tuple<Args2...>>(registry)
			);
		}

		inline static auto view(std::function<bool(glm::vec4)> comparator) {
			return ViewCollection(
				DefaultView<std::tuple<SceneTag<SCENE_ID>, StoreTag<Flags::DEFAULT>, Args1...>, std::tuple<Args2...>>(registry),
				BSHView<std::tuple<SceneTag<SCENE_ID>, StoreTag<Flags::BSH>, Args1...>, std::tuple<Args2...>>(registry, bsh, comparator)
			);
		}
	};

	template<class... Args>
	using InvokeView = View<typename ProcessComponentTypes<Args...>::IncludeTuple, typename ProcessComponentTypes<Args...>::ExcludeTuple>;

	template<class... Args>
	auto view() {
		return InvokeView<Args...>::view();
	}

	template<class... Args>
	auto view(std::function<bool(glm::vec4)> comparator) {
		return InvokeView<Args...>::view(comparator);
	}


	template<size_t FLAG, class... Types>
	std::tuple<Types...> getFromStore(uint64_t entity) {
		return removeTags2(registry.get<SceneTag<SCENE_ID>, StoreTag<FLAG>, Types...>(entity.entity_id));
	}

	template<class... Types>
	std::tuple<Types...> get(uint64_t& entity) {
		return removeTags1(registry.get<SceneTag<SCENE_ID>, Types...>(entity));
	}

private:

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