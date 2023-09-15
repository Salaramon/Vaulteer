#pragma once

#include <glm/glm.hpp>

#include "API/Core.h"
#include "Scene/Entity.h"


struct Position3D : glm::vec3 {
	Position3D() {}
	template<class T1, class T2, class T3>
	Position3D(T1 x, T2 y, T3 z) : glm::vec3(x,y,z) {}
	Position3D(const glm::vec3& vec3) : glm::vec3(vec3) {}
	using glm::vec3::operator=;
};

class Rotation3D : public glm::quat {
public:
	Rotation3D(glm::quat q) : qua<float, glm::qualifier::packed_highp>(q) {
	}

	using glm::quat::qua;
	using glm::quat::operator=;
};

class Properties3D {
public:
	glm::vec3 scale;
	glm::vec3 axisLockDirection;
	bool isAxisLocked;
};


class Object3D : public Entity {
public:
	Position3D* position;
	Rotation3D* rotation;
	Properties3D* properties3D;

	Object3D() :
		position(&add<Position3D>(0.f, 0.f, 0.f)),
		rotation(&add<Rotation3D>(glm::vec3(.0f))),
		properties3D(&add<Properties3D>(Properties3D{
			.scale = glm::vec3(1.0),
			.axisLockDirection = {0.f, 1.f, 0.f},
			.isAxisLocked = false,
		}))
	{}

	Object3D(Object3D&& other) : Entity(std::move(other)),
			position(other.position),
			rotation(other.rotation),
			properties3D(other.properties3D) {
		other.position = nullptr;
		other.rotation = nullptr;
		other.properties3D = nullptr;
	}

	~Object3D() {
		this->remove<Position3D, Rotation3D, Properties3D>();
	}


	void setPosition(glm::vec3 position) {
		*this->position = position;
	}

	void setPosition(float x, float y, float z) {
		setPosition({ x, y, z });
	}

	void setRotation(const glm::vec3 direction) {
		if (properties3D->isAxisLocked) {
			*rotation = glm::quatLookAt(direction, properties3D->axisLockDirection);
		}
		else {
			*rotation = glm::quat(direction);
		}
	}

	void setRotation(const glm::quat quaternion) {
		// TODO this should use the input parameter
		KYSE_ASSERT(false)
		glm::quat qF = *rotation * glm::quat(0, 0, 0, -1) * glm::conjugate(*rotation);
		glm::vec3 result({ qF.x, qF.y, qF.z });
		setRotation(result);
	}

	void setRotation(float yaw, float pitch, float roll) {
		setRotation(glm::vec3(yaw, pitch, roll));
	}

	void setScale(glm::vec3 scale) {
		this->properties3D->scale = scale;
	}

	void move(glm::vec3 direction) {
		*this->position += direction;
	}

	void move(float x, float y, float z) {
		move({ x, y, z });
	}

	void rotate(glm::vec3 direction) {
		glm::quat toMove(direction);
		*this->rotation *= toMove;

		if (properties3D->isAxisLocked) {
			glm::vec3 result = Object3D::frontVector(*rotation);

			*rotation = glm::quatLookAt(result, properties3D->axisLockDirection);
		}
	}

	void rotate(const glm::quat quaternion) {
		glm::quat qF = *rotation * glm::quat(0, 0, 0, -1) * glm::conjugate(*rotation);
		glm::vec3 result({ qF.x, qF.y, qF.z });
		rotate(result);
	}

	void rotate(float x, float y, float z) {
		rotate({ x, y, z });
	}

	void setLockAxis(glm::vec3 direction) {
		properties3D->axisLockDirection = direction;
	}

	void enableAxisLock() {
		properties3D->isAxisLocked = true;
	}

	void disableAxisLock() {
		properties3D->isAxisLocked = false;
	}

	bool hasScaleFactor() const {
		// todo should be cached
		return glm::vec3(1.0f) == properties3D->scale;
	}

	bool hasUniformScale() const {
		// todo should be cached
		return properties3D->scale.x == properties3D->scale.y && properties3D->scale.x == properties3D->scale.z;
	}

	// calculation methods

	static glm::mat4 viewMatrix(const Position3D& position, const Rotation3D& rotation) {

		glm::quat orientationConjugate = glm::conjugate(rotation);
		glm::mat4 mat4FromQuat = glm::mat4_cast(orientationConjugate);
		glm::mat4 translation = glm::translate(glm::mat4(1.0), -(position));

		return mat4FromQuat * translation;
	}

	static glm::mat4 modelMatrix(const Position3D& position, const Rotation3D& rotation, const Properties3D& properties3D) {
		return modelMatrix(position, rotation, properties3D.scale);
	}

	static glm::mat4 modelMatrix(const glm::vec3& position, const glm::quat& rotation, const glm::vec3 scale) {
		return
			glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), scale) *
			glm::mat4_cast(rotation);
	}

	static glm::vec3 frontVector(const Rotation3D& rotation) {
		glm::quat qF = rotation * glm::quat(0, 0, 0, -1) * glm::conjugate(rotation);
		glm::vec3 result({ qF.x, qF.y, qF.z });

		return result;
	}

	static glm::vec3 rightVector(const Rotation3D& rotation) {
		glm::quat qF = rotation * glm::quat(0, -1, 0, 0) * glm::conjugate(rotation);
		glm::vec3 result({ qF.x, qF.y, qF.z });

		return result;
	}

	static glm::vec3 upVector(const Rotation3D& rotation) {
		glm::quat qF = rotation * glm::quat(0, 0, -1, 0) * glm::conjugate(rotation);
		glm::vec3 result({ qF.x, qF.y, qF.z });

		return result;
	}
};
