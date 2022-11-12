#pragma once

#include "API/Layer.h"

class DebugLayer : public Layer {
public:
	DebugLayer() : Layer("DebugLayer") {}
	
	//void onAttach() override;
	//void onUpdate(float timestep) override;

	void onEvent(BaseEvent& e) override;

private:
	bool onKeyboardButtonEvent(KeyboardButtonEvent& e);
};
