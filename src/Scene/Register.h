#pragma once

#include "entt.hpp"

class Register {
protected:
	inline static entt::basic_registry<uint64_t> registry;
};