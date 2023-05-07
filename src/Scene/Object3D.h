#pragma once

#include <glm/glm.hpp>



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
	Object3D() :
		position(&add<Position3D>(0.f, 0.f, 0.f)),
		rotation(&add<Rotation3D>(glm::vec3(0.f))),
		properties3D(&add<Properties3D>(Properties3D{
		.axisLockDirection = {0.f, 1.f, 0.f},
		.isAxisLocked = false}))
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
		if (properties3D->isAxisLocked) {
			glm::quat qF = *rotation * glm::quat(0, 0, 0, -1) * glm::conjugate(*rotation);
			glm::vec3 result({ qF.x, qF.y, qF.z });
			setRotation(result);
		}
		else {
			*rotation = quaternion;
		}
	}

	void setRotation(float yaw, float pitch, float roll) {
		setRotation(glm::vec3(yaw, pitch, roll));
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
	}

	void rotate(float x, float y, float z) {
		rotate({ x, y, z });
	}

	void setLockAxis(glm::vec3 direction) {
		properties3D->axisLockDirection = direction;
	}

	void enableAxisLock(glm::vec3 direction) {
		properties3D->isAxisLocked = true;
	}

	void disableAxisLock(glm::vec3 direction) {
		properties3D->isAxisLocked = false;
	}

	Position3D* position;
	Rotation3D* rotation;
	Properties3D* properties3D;
};


/*
Supplies the functions:
	viewMatrix
	modelMatrix
	frontVector
	rightVector
	upVector
*/
template<class... TupleArgs>
class Object3DUtility : Entity::Restricter<TupleArgs...> {
public:
	Object3DUtility(const Object3D& object3d) :
		position(object3d.position),
		rotation(object3d.rotation),
		properties3D(object3d.properties3D)
	{}

	Object3DUtility(TupleArgs... args) :
		position(Entity::tryGet<Position3D>(std::tie(args...))),
		rotation(Entity::tryGet<Rotation3D>(std::tie(args...))),
		properties3D(Entity::tryGet<Properties3D>(std::tie(args...)))
	{}

	Object3DUtility(std::tuple<TupleArgs...> tuple) :
		position(Entity::tryGet<Position3D>(tuple)),
		rotation(Entity::tryGet<Rotation3D>(tuple)),
		properties3D(Entity::tryGet<Properties3D>(tuple))
	{}

	//Require components: Position3D, Rotation3D
	template<class T = glm::mat4>
	Object3DUtility::EnableWith<T, Position3D, Rotation3D, Properties3D> viewMatrix() const {
		glm::quat orientationConjugate = glm::conjugate(*rotation);
		glm::mat4 mat4FromQuat = glm::mat4_cast(orientationConjugate);
		glm::mat4 translation = glm::translate(glm::mat4(1.0), -(*position));
		glm::mat4 scaled = glm::scale(translation, properties3D->scale);

		glm::mat4 result = translation * scaled * mat4FromQuat;

		return result;
	};

	template<class T = glm::mat4>
	Object3DUtility::EnableWith<T, Position3D, Rotation3D, Properties3D> modelMatrix() const {
		return (
			glm::translate(glm::mat4(1.0f), *position) *
			glm::scale(glm::mat4(1.0f), properties3D->scale) *
			glm::mat4_cast(*rotation)
			);
	}

	template<class T = glm::vec3>
	Object3DUtility::EnableWith<T, Rotation3D> frontVector() const {
		glm::quat qF = *rotation * glm::quat(0, 0, 0, -1) * glm::conjugate(*rotation);
		glm::vec3 result({ qF.x, qF.y, qF.z });

		return result;
	};

	template<class T = glm::vec3>
	Object3DUtility::EnableWith<T, Rotation3D> rightVector() const {
		glm::quat qF = *rotation * glm::quat(0, -1, 0, 0) * glm::conjugate(*rotation);
		glm::vec3 result({ qF.x, qF.y, qF.z });

		return result;
	};
	template<class T = glm::vec3>
	Object3DUtility::EnableWith<T, Rotation3D> upVector() const {
		glm::quat qF = *rotation * glm::quat(0, 0, -1, 0) * glm::conjugate(*rotation);
		glm::vec3 result({ qF.x, qF.y, qF.z });

		return result;
	};


	const Position3D* const position;
	const Rotation3D* const rotation;
	const Properties3D* const properties3D;
};