#include "vpch.h"
#include "LayerStack.h"

#include <ranges>

LayerStack::LayerStack() {}

LayerStack::~LayerStack() {
	for (auto layer : layers | std::ranges::views::reverse) {
		layer->onDetach();
		delete layer;
	}
}

void LayerStack::pushLayer(Layer* layer) {
	layers.emplace_back(layer);
	layer->onAttach();
}

Layer* LayerStack::popLayer() {
	Layer* layer = layers.back();
	layers.pop_back();
	layer->onDetach();
	return layer;
}
