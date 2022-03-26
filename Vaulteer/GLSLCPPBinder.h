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
		if (strings_equal("depth_map_vertex.glsl", a)) { return 1; }
		if (strings_equal("forward_vertex.glsl", a)) { return 2; }
		if (strings_equal("geometry_vertex.glsl", a)) { return 3; }
		if (strings_equal("lightsource_vertex.glsl", a)) { return 4; }
		if (strings_equal("line_vertex.glsl", a)) { return 5; }
		if (strings_equal("shadow_cube_vertex.glsl", a)) { return 6; }
		if (strings_equal("shadow_vertex.glsl", a)) { return 7; }
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
	struct AttributeStructure_depth_map_vertex{
		glm::vec3 aPos;
		glm::vec2 aTexCoords;
		inline static const std::array<size_t, 2> offsets = {0,12};
		inline static const std::array<LocationInfo, 2> locations = {
		LocationInfo(0, "vec3", "aPos", 0, 12),
		LocationInfo(2, "vec2", "aTexCoords", 0, 8)};
	};
	struct AttributeStructure_forward_vertex{
		glm::vec3 aPos;
		glm::vec3 aNormal;
		glm::vec2 aTexCoords;
		inline static const std::array<size_t, 3> offsets = {0,12,24};
		inline static const std::array<LocationInfo, 3> locations = {
		LocationInfo(0, "vec3", "aPos", 0, 12),
		LocationInfo(1, "vec3", "aNormal", 0, 12),
		LocationInfo(2, "vec2", "aTexCoords", 0, 8)};
	};
	struct AttributeStructure_geometry_vertex{
		glm::vec3 aPos;
		glm::vec3 aNormal;
		glm::vec2 aTexCoords;
		glm::vec3 aTangent;
		glm::vec3 aBitangent;
		inline static const std::array<size_t, 5> offsets = {0,12,24,32,44};
		inline static const std::array<LocationInfo, 5> locations = {
		LocationInfo(0, "vec3", "aPos", 0, 12),
		LocationInfo(1, "vec3", "aNormal", 0, 12),
		LocationInfo(2, "vec2", "aTexCoords", 0, 8),
		LocationInfo(3, "vec3", "aTangent", 0, 12),
		LocationInfo(4, "vec3", "aBitangent", 0, 12)};
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
	struct AttributeStructure_line_vertex{
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
	struct AttributeStructure_shadow_vertex{
		glm::vec3 aPos;
		inline static const std::array<size_t, 1> offsets = {0};
		inline static const std::array<LocationInfo, 1> locations = {
		LocationInfo(0, "vec3", "aPos", 0, 12)};
	};

	struct AttributeObject {
		template<char const* str>
		using Get = typename type_list<AttributeStructure_deferred_vertex,AttributeStructure_depth_map_vertex,AttributeStructure_forward_vertex,AttributeStructure_geometry_vertex,AttributeStructure_lightsource_vertex,AttributeStructure_line_vertex,AttributeStructure_shadow_cube_vertex,AttributeStructure_shadow_vertex>::type<str>;
	};

	namespace file_names{
		constexpr char cluster_tile_compute[] = "shaders/cluster_tile_compute.glsl";
		constexpr char deferred_frag[] = "shaders/deferred_frag.glsl";
		constexpr char deferred_vertex[] = "shaders/deferred_vertex.glsl";
		constexpr char depth_map_frag[] = "shaders/depth_map_frag.glsl";
		constexpr char depth_map_vertex[] = "shaders/depth_map_vertex.glsl";
		constexpr char forward_frag[] = "shaders/forward_frag.glsl";
		constexpr char forward_vertex[] = "shaders/forward_vertex.glsl";
		constexpr char geometry_frag[] = "shaders/geometry_frag.glsl";
		constexpr char geometry_vertex[] = "shaders/geometry_vertex.glsl";
		constexpr char lightsource_frag[] = "shaders/lightsource_frag.glsl";
		constexpr char lightsource_vertex[] = "shaders/lightsource_vertex.glsl";
		constexpr char line_frag[] = "shaders/line_frag.glsl";
		constexpr char line_vertex[] = "shaders/line_vertex.glsl";
		constexpr char shadow_cube_frag[] = "shaders/shadow_cube_frag.glsl";
		constexpr char shadow_cube_geometry[] = "shaders/shadow_cube_geometry.glsl";
		constexpr char shadow_cube_vertex[] = "shaders/shadow_cube_vertex.glsl";
		constexpr char shadow_frag[] = "shaders/shadow_frag.glsl";
		constexpr char shadow_vertex[] = "shaders/shadow_vertex.glsl";
	}

	struct VolumeTileAABB{
		VolumeTileAABB(String name)  :
			minPoint{Uniform<glm::vec4>("vec4", String(name + "." + "minPoint"), 0, 16)},
			maxPoint{Uniform<glm::vec4>("vec4", String(name + "." + "maxPoint"), 0, 16)}
		{}

			Uniform<glm::vec4> minPoint;
			Uniform<glm::vec4> maxPoint;
	};

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
			light{BaseLight(String(name + "." + "light"))},
			att{Attenuation(String(name + "." + "att"))},
			position{Uniform<glm::vec3>("vec3", String(name + "." + "position"), 0, 12)},
			radius{Uniform<float>("float", String(name + "." + "radius"), 0, 4)},
			direction{Uniform<glm::vec3>("vec3", String(name + "." + "direction"), 0, 12)},
			angle{Uniform<float>("float", String(name + "." + "angle"), 0, 4)}
		{}

			BaseLight light;
			Attenuation att;
			Uniform<glm::vec3> position;
			Uniform<float> radius;
			Uniform<glm::vec3> direction;
			Uniform<float> angle;
	};

	struct ModelUnitData{
		ModelUnitData(String name)  :
			xDelta{Uniform<int>("int", String(name + "." + "xDelta"), 0, 4)},
			yDelta{Uniform<int>("int", String(name + "." + "yDelta"), 0, 4)},
			wDelta{Uniform<int>("int", String(name + "." + "wDelta"), 0, 4)},
			hDelta{Uniform<int>("int", String(name + "." + "hDelta"), 0, 4)},
			layerDelta{Uniform<int>("int", String(name + "." + "layerDelta"), 0, 4)}
		{}

			Uniform<int> xDelta;
			Uniform<int> yDelta;
			Uniform<int> wDelta;
			Uniform<int> hDelta;
			Uniform<int> layerDelta;
	};

struct cluster_tile_compute {
	struct locations{
		};
		struct uniforms{
			 inline static Uniform<float> zNear{Uniform<float>("float", String("zNear"), 0, 4)};
			 inline static Uniform<float> zFar{Uniform<float>("float", String("zFar"), 0, 4)};
		};
	};

struct deferred_frag {
	struct locations{
		};
		struct uniforms{
			 inline static Uniform<> gPosition{Uniform<>("sampler2D", String("gPosition"), 0, 0)};
			 inline static Uniform<> gNormal{Uniform<>("sampler2D", String("gNormal"), 0, 0)};
			 inline static Uniform<> gColor{Uniform<>("sampler2D", String("gColor"), 0, 0)};
			 inline static Uniform<> shadowMap_0{Uniform<>("sampler2D", String("shadowMap_0"), 0, 0)};
			 inline static Uniform<> shadowMap_1{Uniform<>("sampler2D", String("shadowMap_1"), 0, 0)};
			 inline static Uniform<> shadowMap_2{Uniform<>("sampler2D", String("shadowMap_2"), 0, 0)};
			 inline static Uniform<glm::mat4> lightSpaceMatrices[1]{Uniform<glm::mat4>("mat4", String("lightSpaceMatrices[0]"), 1, 64)};
			 inline static Uniform<float> cascadeFarPlanes[1]{Uniform<float>("float", String("cascadeFarPlanes[0]"), 1, 4)};
			 inline static Uniform<> shadowSpotMap_0{Uniform<>("sampler2D", String("shadowSpotMap_0"), 0, 0)};
			 inline static Uniform<> shadowSpotMap_1{Uniform<>("sampler2D", String("shadowSpotMap_1"), 0, 0)};
			 inline static Uniform<> shadowSpotMap_2{Uniform<>("sampler2D", String("shadowSpotMap_2"), 0, 0)};
			 inline static Uniform<> shadowSpotMap_3{Uniform<>("sampler2D", String("shadowSpotMap_3"), 0, 0)};
			 inline static Uniform<glm::mat4> spotLightSpaceMatrices[1]{Uniform<glm::mat4>("mat4", String("spotLightSpaceMatrices[0]"), 1, 64)};
			 inline static Uniform<> shadowCubeMap_0{Uniform<>("samplerCube", String("shadowCubeMap_0"), 0, 0)};
			 inline static Uniform<> shadowCubeMap_1{Uniform<>("samplerCube", String("shadowCubeMap_1"), 0, 0)};
			 inline static Uniform<> shadowCubeMap_2{Uniform<>("samplerCube", String("shadowCubeMap_2"), 0, 0)};
			 inline static Uniform<> shadowCubeMap_3{Uniform<>("samplerCube", String("shadowCubeMap_3"), 0, 0)};
			 inline static Uniform<float> cubeMapFarPlanes[1]{Uniform<float>("float", String("cubeMapFarPlanes[0]"), 1, 4)};
			 inline static Uniform<glm::vec3> worldCameraPos{Uniform<glm::vec3>("vec3", String("worldCameraPos"), 0, 12)};
			 inline static Uniform<glm::mat4> cameraViewMat{Uniform<glm::mat4>("mat4", String("cameraViewMat"), 0, 64)};
			 inline static DirectionalLight directionalLight{DirectionalLight(String("directionalLight"))};
			 inline static PointLight pointLights[1]{PointLight(String("pointLights[0]"))};
			 inline static SpotLight spotLights[1]{SpotLight(String("spotLights[0]"))};
			 inline static Uniform<float> materialSpecularIntensity{Uniform<float>("float", String("materialSpecularIntensity"), 0, 4)};
			 inline static Uniform<float> materialShininess{Uniform<float>("float", String("materialShininess"), 0, 4)};
			 inline static Uniform<glm::vec3> fogColor{Uniform<glm::vec3>("vec3", String("fogColor"), 0, 12)};
		};
	};

struct deferred_vertex {
	struct locations{
			inline static Location<glm::vec3> aPos{Location <glm::vec3>(0, "vec3", "aPos", 0, 12)};
			inline static Location<glm::vec2> aTexCoords{Location <glm::vec2>(2, "vec2", "aTexCoords", 0, 8)};
		};
		struct uniforms{
		};
	};

struct depth_map_frag {
	struct locations{
		};
		struct uniforms{
			 inline static Uniform<> depthMap{Uniform<>("sampler2D", String("depthMap"), 0, 0)};
		};
	};

struct depth_map_vertex {
	struct locations{
			inline static Location<glm::vec3> aPos{Location <glm::vec3>(0, "vec3", "aPos", 0, 12)};
			inline static Location<glm::vec2> aTexCoords{Location <glm::vec2>(2, "vec2", "aTexCoords", 0, 8)};
		};
		struct uniforms{
		};
	};

struct forward_frag {
	struct locations{
		};
		struct uniforms{
			 inline static Uniform<> textureLib{Uniform<>("sampler2DArray", String("textureLib"), 0, 0)};
			 inline static Uniform<glm::mat4> inverseViewMat{Uniform<glm::mat4>("mat4", String("inverseViewMat"), 0, 64)};
			 inline static Uniform<glm::vec3> cameraPos{Uniform<glm::vec3>("vec3", String("cameraPos"), 0, 12)};
			 inline static Uniform<glm::vec3> lightPos{Uniform<glm::vec3>("vec3", String("lightPos"), 0, 12)};
		};
	};

struct forward_vertex {
	struct locations{
			inline static Location<glm::vec3> aPos{Location <glm::vec3>(0, "vec3", "aPos", 0, 12)};
			inline static Location<glm::vec3> aNormal{Location <glm::vec3>(1, "vec3", "aNormal", 0, 12)};
			inline static Location<glm::vec2> aTexCoords{Location <glm::vec2>(2, "vec2", "aTexCoords", 0, 8)};
		};
		struct uniforms{
			 inline static Uniform<glm::mat4> modelView{Uniform<glm::mat4>("mat4", String("modelView"), 0, 64)};
			 inline static Uniform<glm::mat4> projection{Uniform<glm::mat4>("mat4", String("projection"), 0, 64)};
			 inline static Uniform<glm::mat4> normal{Uniform<glm::mat4>("mat4", String("normal"), 0, 64)};
		};
	};

struct geometry_frag {
	struct locations{
		};
		struct uniforms{
			 inline static ModelUnitData unitTable[6]{ModelUnitData(String("unitTable[0]")), ModelUnitData(String("unitTable[1]")), ModelUnitData(String("unitTable[2]")), ModelUnitData(String("unitTable[3]")), ModelUnitData(String("unitTable[4]")), ModelUnitData(String("unitTable[5]"))};
			 inline static Uniform<> textureLib{Uniform<>("sampler2DArray", String("textureLib"), 0, 0)};
			 inline static Uniform<int> modelNumber{Uniform<int>("int", String("modelNumber"), 0, 4)};
		};
	};

struct geometry_vertex {
	struct locations{
			inline static Location<glm::vec3> aPos{Location <glm::vec3>(0, "vec3", "aPos", 0, 12)};
			inline static Location<glm::vec3> aNormal{Location <glm::vec3>(1, "vec3", "aNormal", 0, 12)};
			inline static Location<glm::vec2> aTexCoords{Location <glm::vec2>(2, "vec2", "aTexCoords", 0, 8)};
			inline static Location<glm::vec3> aTangent{Location <glm::vec3>(3, "vec3", "aTangent", 0, 12)};
			inline static Location<glm::vec3> aBitangent{Location <glm::vec3>(4, "vec3", "aBitangent", 0, 12)};
		};
		struct uniforms{
			 inline static Uniform<glm::mat4> model{Uniform<glm::mat4>("mat4", String("model"), 0, 64)};
			 inline static Uniform<glm::mat4> modelView{Uniform<glm::mat4>("mat4", String("modelView"), 0, 64)};
			 inline static Uniform<glm::mat4> projection{Uniform<glm::mat4>("mat4", String("projection"), 0, 64)};
			 inline static Uniform<glm::mat4> normal{Uniform<glm::mat4>("mat4", String("normal"), 0, 64)};
		};
	};

struct lightsource_frag {
	struct locations{
		};
		struct uniforms{
			 inline static Uniform<glm::vec3> lightColor{Uniform<glm::vec3>("vec3", String("lightColor"), 0, 12)};
		};
	};

struct lightsource_vertex {
	struct locations{
			inline static Location<glm::vec3> aPos{Location <glm::vec3>(0, "vec3", "aPos", 0, 12)};
			inline static Location<glm::vec3> aNormal{Location <glm::vec3>(1, "vec3", "aNormal", 0, 12)};
			inline static Location<glm::vec2> aTexCoords{Location <glm::vec2>(2, "vec2", "aTexCoords", 0, 8)};
		};
		struct uniforms{
			 inline static Uniform<glm::mat4> model{Uniform<glm::mat4>("mat4", String("model"), 0, 64)};
			 inline static Uniform<glm::mat4> view{Uniform<glm::mat4>("mat4", String("view"), 0, 64)};
			 inline static Uniform<glm::mat4> projection{Uniform<glm::mat4>("mat4", String("projection"), 0, 64)};
		};
	};

struct line_frag {
	struct locations{
		};
		struct uniforms{
			 inline static Uniform<glm::vec4> color{Uniform<glm::vec4>("vec4", String("color"), 0, 16)};
		};
	};

struct line_vertex {
	struct locations{
			inline static Location<glm::vec3> aPos{Location <glm::vec3>(0, "vec3", "aPos", 0, 12)};
		};
		struct uniforms{
			 inline static Uniform<glm::mat4> model{Uniform<glm::mat4>("mat4", String("model"), 0, 64)};
			 inline static Uniform<glm::mat4> view{Uniform<glm::mat4>("mat4", String("view"), 0, 64)};
			 inline static Uniform<glm::mat4> projection{Uniform<glm::mat4>("mat4", String("projection"), 0, 64)};
		};
	};

struct shadow_cube_frag {
	struct locations{
		};
		struct uniforms{
			 inline static Uniform<glm::vec3> lightPos{Uniform<glm::vec3>("vec3", String("lightPos"), 0, 12)};
			 inline static Uniform<float> farPlane{Uniform<float>("float", String("farPlane"), 0, 4)};
		};
	};

struct shadow_cube_geometry {
	struct locations{
		};
		struct uniforms{
			 inline static Uniform<glm::mat4> shadowMatrices[6]{Uniform<glm::mat4>("mat4", String("shadowMatrices[0]"), 6, 64), Uniform<glm::mat4>("mat4", String("shadowMatrices[1]"), 6, 64), Uniform<glm::mat4>("mat4", String("shadowMatrices[2]"), 6, 64), Uniform<glm::mat4>("mat4", String("shadowMatrices[3]"), 6, 64), Uniform<glm::mat4>("mat4", String("shadowMatrices[4]"), 6, 64), Uniform<glm::mat4>("mat4", String("shadowMatrices[5]"), 6, 64)};
		};
	};

struct shadow_cube_vertex {
	struct locations{
			inline static Location<glm::vec3> aPos{Location <glm::vec3>(0, "vec3", "aPos", 0, 12)};
		};
		struct uniforms{
			 inline static Uniform<glm::mat4> model{Uniform<glm::mat4>("mat4", String("model"), 0, 64)};
		};
	};

struct shadow_frag {
	struct locations{
		};
		struct uniforms{
		};
	};

struct shadow_vertex {
	struct locations{
			inline static Location<glm::vec3> aPos{Location <glm::vec3>(0, "vec3", "aPos", 0, 12)};
		};
		struct uniforms{
			 inline static Uniform<glm::mat4> lightSpaceMatrix{Uniform<glm::mat4>("mat4", String("lightSpaceMatrix"), 0, 64)};
			 inline static Uniform<glm::mat4> model{Uniform<glm::mat4>("mat4", String("model"), 0, 64)};
		};
	};

};