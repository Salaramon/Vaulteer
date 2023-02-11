#pragma once

#include <memory>
#include <tuple>
#include <type_traits>
#include <vector>

#include "entt.hpp"
#include "Entity.h"
#include "Register.h"
#include "Data Structures/BoundingSphereHierarchy.h"

#include "Debug/Debug.h"

#include <entt.hpp>

class Scene : public Register {
public:
	template<class Type>
	Entity add() {
		Entity entity = registry.create();
		entity
	}

	Entity addWithBSH() {

	}

protected:
	

private:
	BoundingSphereHierarchy<Entity> bsh;
};