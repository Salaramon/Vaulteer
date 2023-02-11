#pragma once

#include "entt.hpp"
#include "Entity.h"

class Register {
public:

protected:
	inline static entt::basic_registry<Entity> registry;
};