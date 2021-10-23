#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <string>
#include <tuple>
#include <array>

namespace Binder {

	struct String : public std::string {
		using std::string::basic_string;
		operator const char*() const{
			return c_str();
		}
	};

	struct LocationInfo {
		LocationInfo() : loc(0), type(""), name(""), array(0), size(0) {}
		LocationInfo(const GLuint loc, String type, String name, const size_t array, const GLint size) : loc(loc), type(type), name(name), array(array), size(size) {}
		const GLuint loc;
		String type;
		String name;
		const GLint size;
		const size_t array;
		size_t flag;
	};

	template<class T = void>
	struct Location : public LocationInfo {
		Location(const GLuint loc, String type, String name, const size_t array, const GLint size) : LocationInfo(loc, type, name, array, size) {}
		using type = T;	};

	struct UniformInfo {
		UniformInfo() : type(""), name(""), array(0), size(0) {}
		UniformInfo(String type, String name, const size_t array, const GLint size) : type(type), name(name), array(array), size(size) {}
		String type;
		String name;
		const size_t array;
		const GLint size;
		size_t flag;
	};

	template<class T = void>
	struct Uniform : public UniformInfo {
		Uniform(String type, String name, const size_t array, const GLint size) : UniformInfo(type, name, array, size) {}
		using type = T;	};

	constexpr bool strings_equal(char const* a, char const* b) {
	return *a == *b && (*a == '\0' || strings_equal(a + 1, b + 1));
}
	constexpr unsigned int get_map_value(char const* a){
		if (strings_equal("deferred_vertex.glsl", a)) { return 0; }
		if (strings_equal("geometry_vertex.glsl", a)) { return 1; }
		if (strings_equal("lightsource_vertex.glsl", a)) { return 2; }
		if (strings_equal("shadow_vertex.glsl", a)) { return 3; }
		if (strings_equal("shadow_cube_vertex.glsl", a)) { return 4; }
		if (strings_equal("shadow_map_vertex.glsl", a)) { return 5; }
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
		inline static const std::array<size_t, 1> offsets = {0};
		inline static const std::array<LocationInfo, 1> locations = {
		LocationInfo(0, "vec3", "aPos", 0, 12)};
	};
	struct AttributeStructure_shadow_cube_vertex{
		glm::vec3 aPos;
		inline static const std::array<size_t, 1> offsets = {0};
		inline static const std::array<LocationInfo, 1> locations = {
		LocationInfo(0, "vec3", "aPos", 0, 12)};
	};
	struct AttributeStructure_shadow_map_vertex{
		glm::vec3 aPos;
		glm::vec2 aTexCoords;
		inline static const std::array<size_t, 2> offsets = {0,12};
		inline static const std::array<LocationInfo, 2> locations = {
		LocationInfo(0, "vec3", "aPos", 0, 12),
		LocationInfo(2, "vec2", "aTexCoords", 0, 8)};
	};

	struct AttributeObject {
		template<char const* str>
		using Get = typename type_list<AttributeStructure_deferred_vertex,AttributeStructure_geometry_vertex,AttributeStructure_lightsource_vertex,AttributeStructure_shadow_vertex,AttributeStructure_shadow_cube_vertex,AttributeStructure_shadow_map_vertex>::type<str>;
	};

	namespace file_names{
		constexpr char deferred_frag[] = "deferred_frag.glsl";
		constexpr char deferred_vertex[] = "deferred_vertex.glsl";
		constexpr char geometry_frag[] = "geometry_frag.glsl";
		constexpr char geometry_vertex[] = "geometry_vertex.glsl";
		constexpr char lightsource_frag[] = "lightsource_frag.glsl";
		constexpr char lightsource_vertex[] = "lightsource_vertex.glsl";
		constexpr char shadow_frag[] = "shadow_frag.glsl";
		constexpr char shadow_vertex[] = "shadow_vertex.glsl";
		constexpr char shadow_cube_vertex[] = "shadow_cube_vertex.glsl";
		constexpr char shadow_cube_geometry[] = "shadow_cube_geometry.glsl";
		constexpr char shadow_cube_frag[] = "shadow_cube_frag.glsl";
		constexpr char shadow_map_vertex[] = "shadow_map_vertex.glsl";
		constexpr char shadow_map_frag[] = "shadow_map_frag.glsl";
	}

	struct Attenuation{
		Attenuation(String name)  :
			aConstant{Uniform<float>("float", String(name + "." + "aConstant"), 0, 4)},
			aLinear{Uniform<float>("float", String(name + "." + "aLinear"), 0, 4)},
			aQuadratic{Uniform<float>("float", String(name + "." + "aQuadratic"), 0, 4)}
		{}

			Uniform<float> aConstant;
			Uniform<float> aLinear;
			Uniform<float> aQuadratic;
	};

	struct BaseLight{
		BaseLight(String name)  :
			color{Uniform<glm::vec3>("vec3", String(name + "." + "color"), 0, 12)},
			ambientIntensity{Uniform<float>("float", String(name + "." + "ambientIntensity"), 0, 4)},
			diffuseIntensity{Uniform<float>("float", String(name + "." + "diffuseIntensity"), 0, 4)}
		{}

			Uniform<glm::vec3> color;
			Uniform<float> ambientIntensity;
			Uniform<float> diffuseIntensity;
	};

	struct DirectionalLight{
		DirectionalLight(String name)  :
			light{BaseLight(String(name + "." + "light"))},
			direction{Uniform<glm::vec3>("vec3", String(name + "." + "direction"), 0, 12)}
		{}

			BaseLight light;
			Uniform<glm::vec3> direction;
	};

	struct PointLight{
		PointLight(String name)  :
			light{BaseLight(String(name + "." + "light"))},
			att{Attenuation(String(name + "." + "att"))},
			position{Uniform<glm::vec3>("vec3", String(name + "." + "position"), 0, 12)},
			radius{Uniform<float>("float", String(name + "." + "radius"), 0, 4)}
		{}

			BaseLight light;
			Attenuation att;
			Uniform<glm::vec3> position;
			Uniform<float> radius;
	};

	struct SpotLight{
		SpotLight(String name)  :
			light{PointLight(String(name + "." + "light"))},
			att{Attenuation(String(name + "." + "att"))},
			position{Uniform<glm::vec3>("vec3", String(name + "." + "position"), 0, 12)},
			radius{Uniform<float>("float", String(name + "." + "radius"), 0, 4)}
		{}

			PointLight light;
			Attenuation att;
			Uniform<glm::vec3> position;
			Uniform<float> radius;
	};

	namespace deferred_frag {
		namespace locations{
		};
		namespace uniforms{
			 inline Uniform<> gPosition(Uniform<>("sampler2D", String("gPosition"), 0, 0));
			 inline Uniform<> gNormal(Uniform<>("sampler2D", String("gNormal"), 0, 0));
			 inline Uniform<> gColor(Uniform<>("sampler2D", String("gColor"), 0, 0));
			 inline Uniform<> shadowMap_0(Uniform<>("sampler2D", String("shadowMap_0"), 0, 0));
			 inline Uniform<> shadowMap_1(Uniform<>("sampler2D", String("shadowMap_1"), 0, 0));
			 inline Uniform<> shadowMap_2(Uniform<>("sampler2D", String("shadowMap_2"), 0, 0));
			 inline Uniform<glm::mat4> lightSpaceMatrices[3](Uniform<glm::mat4>("mat4", String("lightSpaceMatrices[0]"), 3, 64), Uniform<glm::mat4>("mat4", String("lightSpaceMatrices[1]"), 3, 64), Uniform<glm::mat4>("mat4", String("lightSpaceMatrices[2]"), 3, 64));
			 inline Uniform<float> cascadeFarPlanes[3](Uniform<float>("float", String("cascadeFarPlanes[0]"), 3, 4), Uniform<float>("float", String("cascadeFarPlanes[1]"), 3, 4), Uniform<float>("float", String("cascadeFarPlanes[2]"), 3, 4));
			 inline Uniform<> shadowCubeMap_0(Uniform<>("samplerCube", String("shadowCubeMap_0"), 0, 0));
			 inline Uniform<> shadowCubeMap_1(Uniform<>("samplerCube", String("shadowCubeMap_1"), 0, 0));
			 inline Uniform<> shadowCubeMap_2(Uniform<>("samplerCube", String("shadowCubeMap_2"), 0, 0));
			 inline Uniform<> shadowCubeMap_3(Uniform<>("samplerCube", String("shadowCubeMap_3"), 0, 0));
			 inline Uniform<float> cubeMapFarPlanes[4](Uniform<float>("float", String("cubeMapFarPlanes[0]"), 4, 4), Uniform<float>("float", String("cubeMapFarPlanes[1]"), 4, 4), Uniform<float>("float", String("cubeMapFarPlanes[2]"), 4, 4), Uniform<float>("float", String("cubeMapFarPlanes[3]"), 4, 4));
			 inline Uniform<glm::vec3> worldCameraPos(Uniform<glm::vec3>("vec3", String("worldCameraPos"), 0, 12));
			 inline Uniform<glm::mat4> cameraViewMat(Uniform<glm::mat4>("mat4", String("cameraViewMat"), 0, 64));
			 inline DirectionalLight directionalLight{DirectionalLight(String("directionalLight"))};
			 inline PointLight pointLights[4]{PointLight(String("pointLights[0]")), PointLight(String("pointLights[1]")), PointLight(String("pointLights[2]")), PointLight(String("pointLights[3]"))};
			 inline Uniform<float> materialSpecularIntensity(Uniform<float>("float", String("materialSpecularIntensity"), 0, 4));
			 inline Uniform<float> materialShininess(Uniform<float>("float", String("materialShininess"), 0, 4));
			 inline Uniform<glm::vec3> fogColor(Uniform<glm::vec3>("vec3", String("fogColor"), 0, 12));
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
			 inline Uniform<> diffuse1(Uniform<>("sampler2D", String("diffuse1"), 0, 0));
		};
	};

	namespace geometry_vertex {
		namespace locations{
			inline Location<glm::vec3> aPos(0, "vec3", "aPos", 0, 12);
			inline Location<glm::vec3> aNormal(1, "vec3", "aNormal", 0, 12);
			inline Location<glm::vec2> aTexCoords(2, "vec2", "aTexCoords", 0, 8);
		};
		namespace uniforms{
			 inline Uniform<glm::mat4> model(Uniform<glm::mat4>("mat4", String("model"), 0, 64));
			 inline Uniform<glm::mat4> view(Uniform<glm::mat4>("mat4", String("view"), 0, 64));
			 inline Uniform<glm::mat4> projection(Uniform<glm::mat4>("mat4", String("projection"), 0, 64));
			 inline Uniform<glm::mat4> normal(Uniform<glm::mat4>("mat4", String("normal"), 0, 64));
		};
	};

	namespace lightsource_frag {
		namespace locations{
		};
		namespace uniforms{
			 inline Uniform<> diffuse1(Uniform<>("sampler2D", String("diffuse1"), 0, 0));
		};
	};

	namespace lightsource_vertex {
		namespace locations{
			inline Location<glm::vec3> aPos(0, "vec3", "aPos", 0, 12);
			inline Location<glm::vec3> aNormal(1, "vec3", "aNormal", 0, 12);
			inline Location<glm::vec2> aTexCoords(2, "vec2", "aTexCoords", 0, 8);
		};
		namespace uniforms{
			 inline Uniform<glm::mat4> model(Uniform<glm::mat4>("mat4", String("model"), 0, 64));
			 inline Uniform<glm::mat4> view(Uniform<glm::mat4>("mat4", String("view"), 0, 64));
			 inline Uniform<glm::mat4> projection(Uniform<glm::mat4>("mat4", String("projection"), 0, 64));
		};
	};

	namespace shadow_frag {
		namespace locations{
		};
		namespace uniforms{
		};
	};

	namespace shadow_vertex {
		namespace locations{
			inline Location<glm::vec3> aPos(0, "vec3", "aPos", 0, 12);
		};
		namespace uniforms{
			 inline Uniform<glm::mat4> lightSpaceMatrix(Uniform<glm::mat4>("mat4", String("lightSpaceMatrix"), 0, 64));
			 inline Uniform<glm::mat4> model(Uniform<glm::mat4>("mat4", String("model"), 0, 64));
		};
	};

	namespace shadow_cube_vertex {
		namespace locations{
			inline Location<glm::vec3> aPos(0, "vec3", "aPos", 0, 12);
		};
		namespace uniforms{
			 inline Uniform<glm::mat4> model(Uniform<glm::mat4>("mat4", String("model"), 0, 64));
		};
	};

	namespace shadow_cube_geometry {
		namespace locations{
		};
		namespace uniforms{
			 inline Uniform<glm::mat4> shadowMatrices[6](Uniform<glm::mat4>("mat4", String("shadowMatrices[0]"), 6, 64), Uniform<glm::mat4>("mat4", String("shadowMatrices[1]"), 6, 64), Uniform<glm::mat4>("mat4", String("shadowMatrices[2]"), 6, 64), Uniform<glm::mat4>("mat4", String("shadowMatrices[3]"), 6, 64), Uniform<glm::mat4>("mat4", String("shadowMatrices[4]"), 6, 64), Uniform<glm::mat4>("mat4", String("shadowMatrices[5]"), 6, 64));
		};
	};

	namespace shadow_cube_frag {
		namespace locations{
		};
		namespace uniforms{
			 inline Uniform<glm::vec3> lightPos(Uniform<glm::vec3>("vec3", String("lightPos"), 0, 12));
			 inline Uniform<float> farPlane(Uniform<float>("float", String("farPlane"), 0, 4));
		};
	};

	namespace shadow_map_vertex {
		namespace locations{
			inline Location<glm::vec3> aPos(0, "vec3", "aPos", 0, 12);
			inline Location<glm::vec2> aTexCoords(2, "vec2", "aTexCoords", 0, 8);
		};
		namespace uniforms{
		};
	};

	namespace shadow_map_frag {
		namespace locations{
		};
		namespace uniforms{
			 inline Uniform<> depthMap(Uniform<>("sampler2D", String("depthMap"), 0, 0));
		};
	};

};