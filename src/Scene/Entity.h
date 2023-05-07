#pragma once

#include <memory>
#include <vector>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <type_traits>
#include <string_view>
#include <utility>

#include <entt.hpp>

#include "Register.h"


template<class Component>
struct Reference {
public:
	Reference(Component& component) : component(&component) {

	}

	Component const* get() {
		return component;
	}

private:
	Component* const component;
};

class Entity : public Register {
public:
	template<class... Args1>
	struct Restricter {
		template<class... Args2>
		struct _EnableWith {
			template<class T>
			inline static constexpr bool Exists = (std::is_same_v<T, Args1> || ...);

			inline static constexpr bool value = (Exists<Args2> && ...);
		public:

			template<class T>
			using type = std::enable_if_t<value, T>;
		};
	public:
		template<class T, class... Args>
		using EnableWith = _EnableWith<Args...>::template type<T>;
	};


	template<class T1, class T2, class... Args>
	struct TryGetType {
	public:
		using type = std::conditional_t<std::is_same_v<std::remove_cvref_t<std::remove_pointer_t<T1>>, std::remove_cvref_t<std::remove_pointer_t<T2>>>, T2, typename TryGetType<T1, Args...>::type>;
	};

	template<class T1, class T2>
	struct TryGetType<T1, T2> {
	public:
		using type = std::conditional_t<std::is_same_v<std::remove_cvref_t<std::remove_pointer_t<T1>>, std::remove_cvref_t<std::remove_pointer_t<T2>>>, T2, void>;
	};

	template<class T, class... Args>
	inline static T* tryGet(std::tuple<Args...> tuple) {
		constexpr bool typeExists = std::disjunction_v<std::is_same<std::remove_cvref_t<std::remove_pointer_t<T>>, std::remove_cvref_t<std::remove_pointer_t<Args>>>...>;
		if constexpr (typeExists) {
			return &std::get<typename TryGetType<T, Args...>::type>(tuple);
		}
		return nullptr;
	}

	Entity() : entity_id(Register::registry.create()) {}

	Entity(Entity&& other) : entity_id(other.entity_id) {
		other.entity_id = entt::null;
	}

	~Entity() {
		if (entity_id != entt::null)
			Register::registry.destroy(entity_id);
	}

	template<class Type, class... Args>
	Type& add(Args&&... initializerValues) {
		return registry.emplace<Type>(entity_id, std::forward<Args>(initializerValues)...);
	}

	template<class Component, class Type>
	void removeReference(decltype(registry)& registry) {
		registry.remove<Reference<Type>>(entity_id);
		registry.on_destroy<Component>().template disconnect<&removeReference<Reference<Type>, Component>>(entity_id);
	}

	template<class Component, class Type>
	void addReference(Type& subComponent) {
		add<Reference<Type>>(Reference<Type>(subComponent));
		registry.on_destroy<Component>().template connect<&removeReference<Component, Type>>(entity_id);
	}

	template<typename Type, typename... Func>
	void patch(const Entity entity, Func&&... func) {
		registry.patch<Type>(entity, func);
	}

	template<class Type, class... Args>
	void replace(Entity entity, Args&&... initializerValues) {
		registry.replace<Type>(entity, std::forward<Args>(initializerValues)...);
	}

	template<class... Types>
	void remove() {
		if (entity_id != entt::null)
			registry.remove<Types...>(entity_id);
	}

	
	uint64_t entity_id;

private:

	struct Types {
	public:
		inline static constexpr uint64_t Model = 0x0100000000000000;
		inline static constexpr uint64_t Light = 0x0200000000000000;
	};
};