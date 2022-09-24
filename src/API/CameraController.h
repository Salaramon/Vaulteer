#pragma once

#include "Camera.h"
#include "Event.h"

class CameraController {
public:

	CameraController();
	CameraController(Camera* camera);

	void setCamera(Camera* camera);

	void onUpdate();

private:
	Camera* camera;
};

