#pragma once

#include <iostream>
#include <string>

#include "Event.h"

/*
 * Interface for layer component. Used for propagation of events + organization of rendering
 */
class Layer {
public:
	Layer(std::string name) : name(std::move(name)) {}
	Layer(Layer& other) = delete;
	Layer(Layer&& other) noexcept : name(other.name) {}

	virtual ~Layer() {}

	virtual void onAttach() {
		std::cout << "running default attach for layer: " << name << std::endl;
	}
	virtual void onDetach() {
		std::cout << "running default detach for layer: " << name << std::endl;
	}

	virtual void onUpdate(float timestep) {}

private:
	const std::string name;
};

