#pragma once

#include "Camera.h"
#include "Events/Event.h"

class CameraController {
public:

	CameraController();
	CameraController(Camera* camera);

	void setCamera(Camera* camera);

	void onUpdate(float timestep);
	bool onMouseMoveEvent(MouseMoveEvent& e);
	bool onKeyboardButtonEvent(KeyboardButtonEvent& e);

private:
	Camera* camera;

	bool dirty = false;

	float sens = 0.155f;
	float speed = 20.0f;
};

