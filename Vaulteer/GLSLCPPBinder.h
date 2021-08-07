#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <tuple>
#include <array>

namespace Binder {

	struct LocationInfo {
		LocationInfo() : loc(0), type(""), name(""), array(0), size(0) {}
		LocationInfo(const GLuint loc, const char* type, const char* name, const size_t array, const GLint size) : loc(loc), type(type), name(name), array(array), size(size) {}
		const GLuint loc;
		const char* type;
		const char* name;
		const GLint size;
		const size_t array;
		size_t flag;
	};

	template<class T = void>
	struct Location : public LocationInfo {
		Location(const GLuint loc, const char* type, const char* name, const size_t array, const GLint size) : LocationInfo(loc, type, name, array, size) {}
		using type = T;	};

	struct UniformInfo {
		UniformInfo() : type(""), name(""), array(0), size(0) {}
		UniformInfo(const char* type, const char* name, const size_t array, const GLint size) : type(type), name(name), array(array), size(size) {}
		const char* type;
		const char* name;
		const size_t array;
		const GLint size;
		size_t flag;
	};

	template<class T = void>
	struct Uniform : public UniformInfo {
		Uniform(const char* type, const char* name, const size_t array, const GLint size) : UniformInfo(type, name, array, size) {}
		using type = T;	};

	constexpr bool strings_equal(char const* a, char const* b) {
	return *a == *b && (*a == '\0' || strings_equal(a + 1, b + 1));
}
	constexpr unsigned int get_map_value(char const* a){
		if (strings_equal("vertex.shader", a)) { return 0; }
	};

	template<class... Args>
	struct type_list
{
	template <char const* str>
	using type = typename std::tuple_element<get_map_value(str), std::tuple<Args...>>::type;
	};
	struct AttributeStructure_vertex{
		glm::vec3 aPos;
		glm::vec3 aNormal;
		glm::vec2 aTexCoords;
		glm::mat4 instanceMatrix;
		inline static const std::array<size_t, 4> offsets = {0,12,24,32};
		inline static const std::array<LocationInfo, 4> locations = {
		LocationInfo(0, "vec3", "aPos", 0, 12),
		LocationInfo(1, "vec3", "aNormal", 0, 12),
		LocationInfo(2, "vec2", "aTexCoords", 0, 8),
		LocationInfo(3, "mat4", "instanceMatrix", 0, 64)};
	};

	struct AttributeObject {
		template<char const* str>
		using Get = typename type_list<AttributeStructure_vertex>::type<str>;
	};

	namespace file_names{
		constexpr char fragment[] = "fragment.shader";
		constexpr char vertex[] = "vertex.shader";
	}

	namespace fragment {
		namespace locations{
		};
		namespace uniforms{
			inline Uniform<> diffuse1("sampler2D", "diffuse1", 0, 0);
		};
	};

	namespace vertex {
		namespace locations{
			inline Location<glm::vec3> aPos(0, "vec3", "aPos", 0, 12);
			inline Location<glm::vec3> aNormal(1, "vec3", "aNormal", 0, 12);
			inline Location<glm::vec2> aTexCoords(2, "vec2", "aTexCoords", 0, 8);
			inline Location<glm::mat4> instanceMatrix(3, "mat4", "instanceMatrix", 0, 64);
		};
		namespace uniforms{
			inline Uniform<glm::mat4> view("mat4", "view", 0, 64);
			inline Uniform<glm::mat4> projection("mat4", "projection", 0, 64);
		};
	};

};//>>END<<