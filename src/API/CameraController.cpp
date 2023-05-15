#include "vpch.h"
#include "CameraController.h"


CameraController::CameraController() : camera(nullptr) {}

CameraController::CameraController(Camera* camera) : camera(camera) {
	assert(camera != nullptr);
}

void CameraController::setCamera(Camera* camera) {
	this->camera = camera;
}

void CameraController::onUpdate(float timestep) {

	glm::vec3 worldUp = camera->properties3D->axisLockDirection;
	
	if (Event::isDown(KeyboardKey::SPACE)) {
		camera->move(worldUp * timestep * speed);
	}
	if (Event::isDown(KeyboardKey::LEFT_CONTROL)) {
		camera->move(-worldUp * timestep * speed);
	}
	if (Event::isDown(KeyboardKey::W)) {
		camera->move(Camera::frontVector(*camera->rotation) * timestep * speed);
	}
	if (Event::isDown(KeyboardKey::S)) {
		camera->move(-Camera::frontVector(*camera->rotation) * timestep * speed);
	}
	if (Event::isDown(KeyboardKey::A)) {
		camera->move(Camera::rightVector(*camera->rotation) * timestep * speed);
	}
	if (Event::isDown(KeyboardKey::D)) {
		camera->move(-Camera::rightVector(*camera->rotation) * timestep * speed);
	}
	if (Event::isDown(KeyboardKey::E)) {
		camera->rotate(0, 0, timestep * (speed / sens));
	}
	if (Event::isDown(KeyboardKey::Q)) {
		camera->rotate(0, 0, timestep * -(speed / sens));
	}

	// Instead of using this concept of applying rotation when transformation values are set,
	// it should just be set when the use calls the function.
	// There is no benefit to do it any other way, as "allowing" the user to call such functions multiple times
	// just encourages bad practice (unecessarily calling a function multiple times).
	// Therefor encouraging to calculate the value before calling the function to set and apply the transformation is the better practice.
	/*
	if (camera->isRotationDirty())
		camera->applyRotation();
	*/
}

bool CameraController::onMouseMoveEvent(MouseMoveEvent& e) {
	camera->rotate(e.motion.delta.y * sens / 1.35f, e.motion.delta.x * sens, 0);
	return true;
}

bool CameraController::onKeyboardButtonEvent(KeyboardButtonEvent& e) {
	return false;
}
