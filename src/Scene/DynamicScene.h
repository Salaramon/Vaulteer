#pragma once

#include <memory>
#include <tuple>
#include <type_traits>
#include <vector>

#include <entt.hpp>

#include "Scene/Scene.h"

#include "Debug/Debug.h"

#include "Entity.h"
#include "Register.h"


class DynamicStore : public Register
{
public:
	DynamicStore() {}
	


	uint32_t allocate() {
		return registry.create();
		
	}

	void add() {
		
	}

	void deallocate(uint32_t val) {
		registry.destroy(val);
	}

private:
	
};