#pragma once

#include <vector>

#include "Layer.h"

class LayerStack {
public:
	LayerStack() = default;

	~LayerStack() {
		for (auto layer : layers | std::ranges::views::reverse) {
			layer->onDetach(layer->context);
		}
	}

	void pushLayer(LayerFunctions* layer) {
		layers.emplace_back(layer);
		layer->onAttach(layer->context);
	}

	LayerFunctions* popLayer() {
		LayerFunctions* layer = layers.back();
		layers.pop_back();
		layer->onDetach(layer->context);
		return layer;
	}
	
	std::vector<LayerFunctions*>::iterator begin() { return layers.begin(); }
	std::vector<LayerFunctions*>::iterator end() { return layers.end(); }
	
	std::vector<LayerFunctions*>::reverse_iterator rbegin() { return layers.rbegin(); }
	std::vector<LayerFunctions*>::reverse_iterator rend() { return layers.rend(); }

private:
	std::vector<LayerFunctions*> layers;
};
