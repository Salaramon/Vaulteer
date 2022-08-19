#pragma once

#include "Model/Model.h"

template<class Model>
class Transparent {
public:
	template<class Model>
	Transparent(Model&& m) : model(std::move(m)) {}

	Transparent(const Transparent& other) : model(other.model) {}
	Transparent(Transparent&& other) : model(std::move(other.model)) {}

	template<class Model>
	operator Model() const { return model; }

	template<class Model>
	operator Model& () const { return model; }

	template<class Model>
	operator Model && () const { return model; }

	Model model;
};

using TransparentModel = Transparent<Model<ModelData>>;