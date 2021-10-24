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

template<class Container, class T, class... Rest>
class _Scene<Container, T, Rest...> : public std::enable_if<(sizeof...(Rest) != 1), _Scene<Container, Rest...>>::type, public _Scene<Container, T> {
public:
	_Scene(Container& containerDispatch) : _Scene<Container, T>::_Scene(containerDispatch) {}
	using _Scene<Container, T>::addObject;
};

template<class Container, class T>
class _Scene<Container, T> {
public:
	_Scene(Container& containerDispatch) : objectVectors(containerDispatch) {}

	T* addObject(T&& object) {
		std::get<std::vector<std::unique_ptr<T>>>(objectVectors).emplace_back(std::make_unique<T>(std::move(object)));
	}

private:
	Container& objectVectors;
};

template<class... SceneObjects>
class Scene : public _Scene<SceneObjects...>
{
public:
	SceneContainer<SceneObjects...> objectVectors;
	
	Scene() : _Scene<SceneObjects...>::_Scene(objectVectors) {}

	using _Scene<SceneObjects...>::addObject;

private:
	
};

