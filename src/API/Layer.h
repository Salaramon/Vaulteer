#pragma once

#include <iostream>
#include <string>

#include "Events/Event.h"

/*
 * Interface for layer component. Used for propagation of events + organization of rendering
 */
class Layer {
public:
	Layer(std::string name) : name(std::move(name)) {}
	Layer(Layer& other);
	Layer(Layer&& other) noexcept : name(other.name) {}

	virtual ~Layer() {}

	virtual void onAttach() {
		Log::trace("running default attach for layer: {}", name);
	}
	virtual void onDetach() {
		Log::trace("running default detach for layer: {}", name);
	}
	
	virtual void onEvent(BaseEvent& e) {}

	virtual void onUpdate(float timestep) {}

protected:
	const std::string name;
};

