#pragma once

#include <memory>
#include <vector>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <type_traits>
#include <string_view>

#include <entt.hpp>

#include "Register.h"


template<size_t N>
struct CompileTimeText {
	consteval CompileTimeText(consteval const char (&str)[]) {
		std::copy_n(str, N, value);
	}

	consteval char[]() {
		return text;
	}

	consteval char text[N];
};


template<CompileTimeText id>
class Entity : public Register {
public: 
	Entity() {
		Register::registry.create();
		registry.emplace<()
	}

	std::string_view name;
	uint64_t entity_type;

	struct Types {
	public:
		inline static constexpr uint64_t Model = 0x0100000000000000;
		inline static constexpr uint64_t Light = 0x0200000000000000;
	};
};