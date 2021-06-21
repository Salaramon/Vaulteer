#pragma once

#include <glm/glm.hpp>
#include <array>

namespace Binder {

	struct Location {
		Location(const size_t id, const size_t size, const size_t array) : id(id), size(size), array(array) {}
		const size_t id;
		const size_t size;
		size_t array;
		size_t flag;
	};
	struct Uniform {
		Uniform(const char* name, const char* type, const size_t array) : name(name), type(type), array(array) {}
		const char* name;
		const char* type;
		size_t array;
		size_t flag;
	};

	namespace fragment {
		namespace locations{
		};
		namespace uniforms{
			inline Uniform diffuse1("diffuse1", "sampler2D", 1);
		};
	};

	namespace vertex {
		namespace locations{
			inline Location aPos(0, 3, 1);
			inline Location aNormal(1, 3, 1);
			inline Location aTexCoords(2, 2, 1);
		};
		namespace uniforms{
			inline Uniform view("view", "mat4", 1);
			inline Uniform projection("projection", "mat4", 1);
			inline Uniform model("model", "mat4", 1);
		};
	};

};//>>END<<

typedef Binder::Uniform Uniform;
typedef Binder::Location Location;