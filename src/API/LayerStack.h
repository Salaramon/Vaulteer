#pragma once

#include <vector>

#include "Layer.h"

class LayerStack {
public:
	LayerStack();
	~LayerStack();

	void pushLayer(Layer* layer);
	Layer* popLayer();
	
	std::vector<Layer*>::iterator begin() { return layers.begin(); }
	std::vector<Layer*>::iterator end() { return layers.end(); }
	
	std::vector<Layer*>::reverse_iterator rbegin() { return layers.rbegin(); }
	std::vector<Layer*>::reverse_iterator rend() { return layers.rend(); }

private:
	std::vector<Layer*> layers;
};
