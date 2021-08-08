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
		if (strings_equal("deferred_vertex.glsl", a)) { return 0; }
		if (strings_equal("geometry_vertex.glsl", a)) { return 1; }
		if (strings_equal("lightsource_vertex.glsl", a)) { return 2; }
		if (strings_equal("shadow_vertex.glsl", a)) { return 3; }
	};

	template<class... Args>
	struct type_list
{
	template <char const* str>
	using type = typename std::tuple_element<get_map_value(str), std::tuple<Args...>>::type;
	};
	struct AttributeStructure_deferred_vertex{
		glm::vec3 aPos;
		glm::vec2 aTexCoords;
		inline static const std::array<size_t, 2> offsets = {0,12};
		inline static const std::array<LocationInfo, 2> locations = {
		LocationInfo(0, "vec3", "aPos", 0, 12),
		LocationInfo(2, "vec2", "aTexCoords", 0, 8)};
	};
	struct AttributeStructure_geometry_vertex{
		glm::vec3 aPos;
		glm::vec3 aNormal;
		glm::vec2 aTexCoords;
		inline static const std::array<size_t, 3> offsets = {0,12,24};
		inline static const std::array<LocationInfo, 3> locations = {
		LocationInfo(0, "vec3", "aPos", 0, 12),
		LocationInfo(1, "vec3", "aNormal", 0, 12),
		LocationInfo(2, "vec2", "aTexCoords", 0, 8)};
	};
	struct AttributeStructure_lightsource_vertex{
		glm::vec3 aPos;
		glm::vec3 aNormal;
		glm::vec2 aTexCoords;
		inline static const std::array<size_t, 3> offsets = {0,12,24};
		inline static const std::array<LocationInfo, 3> locations = {
		LocationInfo(0, "vec3", "aPos", 0, 12),
		LocationInfo(1, "vec3", "aNormal", 0, 12),
		LocationInfo(2, "vec2", "aTexCoords", 0, 8)};
	};
	struct AttributeStructure_shadow_vertex{
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
		using Get = typename type_list<AttributeStructure_deferred_vertex,AttributeStructure_geometry_vertex,AttributeStructure_lightsource_vertex,AttributeStructure_shadow_vertex>::type<str>;
	};

	namespace file_names{
		constexpr char deferred_frag[] = "deferred_frag.glsl";
		constexpr char deferred_vertex[] = "deferred_vertex.glsl";
		constexpr char geometry_frag[] = "geometry_frag.glsl";
		constexpr char geometry_vertex[] = "geometry_vertex.glsl";
		constexpr char light_frag[] = "light_frag.glsl";
		constexpr char light_vertex[] = "light_vertex.glsl";
		constexpr char lightsource_frag[] = "lightsource_frag.glsl";
		constexpr char lightsource_vertex[] = "lightsource_vertex.glsl";
		constexpr char shadow_frag[] = "shadow_frag.glsl";
		constexpr char shadow_vertex[] = "shadow_vertex.glsl";
	}

	namespace deferred_frag {
		namespace locations{
		};
		namespace uniforms{
			inline Uniform<> gPosition("sampler2D", "gPosition", 0, 0);
			inline Uniform<> gNormal("sampler2D", "gNormal", 0, 0);
			inline Uniform<> gColor("sampler2D", "gColor", 0, 0);
			inline Uniform<glm::vec3> worldCameraPos("vec3", "worldCameraPos", 0, 12);
			inline Uniform<float> materialSpecularIntensity("float", "materialSpecularIntensity", 0, 4);
			inline Uniform<float> materialShininess("float", "materialShininess", 0, 4);
		};
	};

	namespace deferred_vertex {
		namespace locations{
			inline Location<glm::vec3> aPos(0, "vec3", "aPos", 0, 12);
			inline Location<glm::vec2> aTexCoords(2, "vec2", "aTexCoords", 0, 8);
		};
		namespace uniforms{
		};
	};

	namespace geometry_frag {
		namespace locations{
		};
		namespace uniforms{
			inline Uniform<> diffuse1("sampler2D", "diffuse1", 0, 0);
		};
	};

	namespace geometry_vertex {
		namespace locations{
			inline Location<glm::vec3> aPos(0, "vec3", "aPos", 0, 12);
			inline Location<glm::vec3> aNormal(1, "vec3", "aNormal", 0, 12);
			inline Location<glm::vec2> aTexCoords(2, "vec2", "aTexCoords", 0, 8);
		};
		namespace uniforms{
			inline Uniform<glm::mat4> model("mat4", "model", 0, 64);
			inline Uniform<glm::mat4> view("mat4", "view", 0, 64);
			inline Uniform<glm::mat4> projection("mat4", "projection", 0, 64);
			inline Uniform<glm::mat4> normal("mat4", "normal", 0, 64);
		};
	};

	namespace light_frag {
		namespace locations{
		};
		namespace uniforms{
		};
	};

	namespace light_vertex {
		namespace locations{
		};
		namespace uniforms{
		};
	};

	namespace lightsource_frag {
		namespace locations{
		};
		namespace uniforms{
			inline Uniform<glm::vec3> lightColor("vec3", "lightColor", 0, 12);
		};
	};

	namespace lightsource_vertex {
		namespace locations{
			inline Location<glm::vec3> aPos(0, "vec3", "aPos", 0, 12);
			inline Location<glm::vec3> aNormal(1, "vec3", "aNormal", 0, 12);
			inline Location<glm::vec2> aTexCoords(2, "vec2", "aTexCoords", 0, 8);
		};
		namespace uniforms{
			inline Uniform<glm::mat4> model("mat4", "model", 0, 64);
			inline Uniform<glm::mat4> view("mat4", "view", 0, 64);
			inline Uniform<glm::mat4> projection("mat4", "projection", 0, 64);
		};
	};

	namespace shadow_frag {
		namespace locations{
		};
		namespace uniforms{
			inline Uniform<> shadowMap("sampler2D", "shadowMap", 0, 0);
		};
	};

	namespace shadow_vertex {
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

};