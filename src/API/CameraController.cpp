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
	glm::vec3 worldUp = camera->getWorldUp();
	
	if (Event::isDown(KeyboardKey::SPACE)) {
		camera->move(worldUp * timestep * speed);
	}
	if (Event::isDown(KeyboardKey::LEFT_CONTROL)) {
		camera->move(-worldUp * timestep * speed);
	}
	if (Event::isDown(KeyboardKey::W)) {
		camera->move(camera->getFront() * timestep * speed);
	}
	if (Event::isDown(KeyboardKey::S)) {
		camera->move(-camera->getFront() * timestep * speed);
	}
	if (Event::isDown(KeyboardKey::A)) {
		camera->move(camera->getRight() * timestep * speed);
	}
	if (Event::isDown(KeyboardKey::D)) {
		camera->move(-camera->getRight() * timestep * speed);
	}
	if (Event::isDown(KeyboardKey::E)) {
		camera->rotate(0, 0, timestep * (speed / sens));
	}
	if (Event::isDown(KeyboardKey::Q)) {
		camera->rotate(0, 0, timestep * -(speed / sens));
	}

	if (camera->isRotationDirty())
		camera->applyRotation();
}

bool CameraController::onMouseMoveEvent(MouseMoveEvent& e) {
	camera->rotate(e.motion.delta.x * sens / 1.35f, e.motion.delta.y * sens, 0);
	return true;
}

bool CameraController::onKeyboardButtonEvent(KeyboardButtonEvent& e) {
	return false;
}
