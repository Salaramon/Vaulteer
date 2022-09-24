#include "vpch.h"
#include "CameraController.h"


CameraController::CameraController() : camera(nullptr) {}

CameraController::CameraController(Camera* camera) : camera(camera) {}

void CameraController::setCamera(Camera* camera) {
	this->camera = camera;
}

void CameraController::onUpdate() {
	assert(camera != nullptr);

	glm::vec3 worldUp = camera->getWorldUp();
	
	float sens = 0.155f;
	float speed = 20.0f;

	/*
	camera->rotate(Event::CURSOR::X.delta() * sens / 1.35, Event::CURSOR::Y.delta() * sens, 0);
	if (Event::isKeyDown(Event::KEY::SPACE)) {
		camera->move(worldUp * (float)Event::delta() * speed);
	}
	if (Event::isKeyDown(Event::KEY::LEFT_CONTROL)) {
		camera->move(-worldUp * (float)Event::delta() * speed);
	}
	if (Event::isKeyDown(Event::KEY::W)) {
		camera->move(camera->getFront() * (float)Event::delta() * speed);
	}
	if (Event::isKeyDown(Event::KEY::S)) {
		camera->move(-camera->getFront() * (float)Event::delta() * speed);
	}
	if (Event::isKeyDown(Event::KEY::A)) {
		camera->move(camera->getRight() * (float)Event::delta() * speed);
	}
	if (Event::isKeyDown(Event::KEY::D)) {
		camera->move(-camera->getRight() * (float)Event::delta() * speed);
	}
	if (Event::isKeyDown(Event::KEY::E)) {
		camera->rotate(0, 0, (float)Event::delta() * (speed / sens));
	}
	if (Event::isKeyDown(Event::KEY::Q)) {
		camera->rotate(0, 0, -(float)Event::delta() * (speed / sens));
	}
	*/

	camera->apply();
}
