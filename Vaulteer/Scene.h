#pragma once

#include <vector>
#include <memory>
#include <tuple>
#include <type_traits>

#include "Camera.h"
//#include "SceneLayer.h"


template<class... SceneObjects>
using SceneContainer = std::tuple<typename std::vector<std::unique_ptr<SceneObjects>>...>;

template<class... SceneObjects>
class _Scene {};

template<class Container, class T>
class _Scene<Container, T> {
public:
	_Scene(Container& containerDispatch) : objectVectors(containerDispatch) {}

	T* addObject(T&& object) {
		std::get<std::vector<std::unique_ptr<T>>>(objectVectors).emplace_back(std::make_unique<T>(std::move(object)));
		return std::get<std::vector<std::unique_ptr<T>>>(objectVectors).back().get();
	}

private:
	Container& objectVectors;
};

template<class... SceneObjects>
class Scene : public _Scene<SceneContainer<SceneObjects...>, SceneObjects>...
{
public:
	SceneContainer<SceneObjects...> objectVectors;
	
	Scene() : _Scene<SceneContainer<SceneObjects...>, SceneObjects...>::_Scene(objectVectors) {}

	using _Scene<SceneContainer<SceneObjects...>, SceneObjects...>::addObject;

private:
	
};

