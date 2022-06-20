#pragma once

#include "Model/Model.h"

template<class Model>
class Opaque {
public:
	template<class Model>
	Opaque(Model&& m) : model(std::move(m)) {}

	Opaque(Opaque& other) : model(other.model) {}
	Opaque(Opaque&& other) : model(std::move(other.model)) {}

	template<class Model>
	operator Model() const { return model; }

	template<class Model>
	operator Model&() const { return model; }

	template<class Model>
	operator Model&&() const { return model; }

	Model model;
};

using OpaqueModel = Opaque<Model<ModelData>>;