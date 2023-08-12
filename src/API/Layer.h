#pragma once

#include <iostream>
#include <string>

#include "Events/Event.h"

namespace LayerDefaults {
	inline void no_attach(void*) {}
	inline void no_detach(void*) {}
	inline void no_event(void*, BaseEvent&) {}
	inline void no_update(void*, float) {}
}

struct LayerFunctions {
	void* context;
	void (*onAttach)(void*) = &LayerDefaults::no_attach;
	void (*onDetach)(void*) = &LayerDefaults::no_detach;
	void (*onEvent)(void*, BaseEvent&) = &LayerDefaults::no_event;
	void (*onUpdate)(void*, float) = &LayerDefaults::no_update;
};

