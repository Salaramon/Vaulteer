#pragma once

#include <tuple>

#include "Scene.h"

template<class... AllowedData>
class RendererPrerequisites {
public:
	using SceneData = std::tuple<AllowedData...>;
	template<template<class> class Container>
	using BaseScene = Scene<Container, AllowedData...>;
};