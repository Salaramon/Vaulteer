#pragma once

#include <memory>

#include "Renderer/Shader.h"

#include <GLSLCPPBinder.h>

template <class... ShaderBinders>
struct ShaderUniformPair : public ShaderBinders... {
public:
};

using BlendingShader = const ShaderUniformPair<Binder::blending_vertex, Binder::blending_frag>;
using BlendingCompositeShader = const ShaderUniformPair<Binder::blending_composite_vertex, Binder::blending_composite_frag>;
using ForwardShader = const ShaderUniformPair<Binder::forward_vertex, Binder::forward_frag>;
using LineShader = const ShaderUniformPair<Binder::line_vertex, Binder::line_frag>;
using DeferredDirShader = const ShaderUniformPair<Binder::deferred_directional_vertex, Binder::deferred_directional_frag>;
using DeferredPointShader = const ShaderUniformPair<Binder::deferred_point_vertex, Binder::deferred_point_frag>;
using GeometryShader = const ShaderUniformPair<Binder::geometry_vertex, Binder::geometry_frag>;
using VolumeShader = const ShaderUniformPair<Binder::volume_vertex, Binder::volume_geom, Binder::volume_frag>;

using ClusterTileComputeShader = const ShaderUniformPair<Binder::cluster_tile_compute>;

#define KYSE_CHECK_SHADER_FILE_NAME(shaderType) \
	if constexpr (std::is_same_v<ShaderBinder, Binder::shaderType ## _vertex>) { return Binder::file_names::shaderType ## _vertex; } \
	if constexpr (std::is_same_v<ShaderBinder, Binder::shaderType ## _frag>) { return Binder::file_names::shaderType ## _frag; }

template <class ShaderBinder>
constexpr const char* getShaderFile() {
	KYSE_CHECK_SHADER_FILE_NAME(blending)
	KYSE_CHECK_SHADER_FILE_NAME(blending_composite)
	KYSE_CHECK_SHADER_FILE_NAME(forward)
	KYSE_CHECK_SHADER_FILE_NAME(line)
	KYSE_CHECK_SHADER_FILE_NAME(deferred_directional)
	KYSE_CHECK_SHADER_FILE_NAME(deferred_point)
	KYSE_CHECK_SHADER_FILE_NAME(geometry)
	KYSE_CHECK_SHADER_FILE_NAME(volume)

	if constexpr (std::is_same_v<ShaderBinder, Binder::volume_geom>) { return Binder::file_names::volume_geom; }

	if constexpr (std::is_same_v<ShaderBinder, Binder::cluster_tile_compute>) { return Binder::file_names::cluster_tile_compute; }

	assert(true, "No shader file found for input");
	return "";
}

template <class ShaderBinder>
constexpr const GLenum getShaderType() {
	if constexpr (std::is_same_v<ShaderBinder, Binder::blending_vertex>) { return GL_VERTEX_SHADER; }
	if constexpr (std::is_same_v<ShaderBinder, Binder::blending_frag>) { return GL_FRAGMENT_SHADER; }

	if constexpr (std::is_same_v<ShaderBinder, Binder::blending_composite_vertex>) { return GL_VERTEX_SHADER; }
	if constexpr (std::is_same_v<ShaderBinder, Binder::blending_composite_frag>) { return GL_FRAGMENT_SHADER; }

	if constexpr (std::is_same_v<ShaderBinder, Binder::forward_vertex>) { return GL_VERTEX_SHADER; }
	if constexpr (std::is_same_v<ShaderBinder, Binder::forward_frag>) { return GL_FRAGMENT_SHADER; }

	if constexpr (std::is_same_v<ShaderBinder, Binder::line_vertex>) { return GL_VERTEX_SHADER; }
	if constexpr (std::is_same_v<ShaderBinder, Binder::line_frag>) { return GL_FRAGMENT_SHADER; }

	if constexpr (std::is_same_v<ShaderBinder, Binder::deferred_point_vertex>) { return GL_VERTEX_SHADER; }
	if constexpr (std::is_same_v<ShaderBinder, Binder::deferred_point_frag>) { return GL_FRAGMENT_SHADER; }

	if constexpr (std::is_same_v<ShaderBinder, Binder::deferred_directional_vertex>) { return GL_VERTEX_SHADER; }
	if constexpr (std::is_same_v<ShaderBinder, Binder::deferred_directional_frag>) { return GL_FRAGMENT_SHADER; }

	if constexpr (std::is_same_v<ShaderBinder, Binder::geometry_vertex>) { return GL_VERTEX_SHADER; }
	if constexpr (std::is_same_v<ShaderBinder, Binder::geometry_frag>) { return GL_FRAGMENT_SHADER; }

	if constexpr (std::is_same_v<ShaderBinder, Binder::volume_vertex>) { return GL_VERTEX_SHADER; }
	if constexpr (std::is_same_v<ShaderBinder, Binder::volume_geom>) { return GL_GEOMETRY_SHADER; }
	if constexpr (std::is_same_v<ShaderBinder, Binder::volume_frag>) { return GL_FRAGMENT_SHADER; }

	if constexpr (std::is_same_v<ShaderBinder, Binder::cluster_tile_compute>) { return GL_COMPUTE_SHADER; }
	
	assert(true, "No shader type found for input");
	return 0;
}

template <class ReturnType, class ShaderUniformType>
struct getShaderInfo {};

template <class ShaderUniformType>
struct getShaderInfo<const char*, ShaderUniformType> {
	constexpr const char* operator()() {
		return getShaderFile<ShaderUniformType>();
	}
};

template <class ShaderUniformType>
struct getShaderInfo<const GLenum, ShaderUniformType> {
	constexpr GLenum operator()() {
		return getShaderType<ShaderUniformType>();
	}
};

template <size_t enumeration, class ShaderUniformType>
struct ShaderInfoFromType {
public:
	const size_t n = enumeration;
	using Type = ShaderUniformType;
	std::conditional_t<enumeration % 2 == 0, const char*, const GLenum> value = getShaderInfo<std::conditional_t<enumeration % 2 == 0, const char*, const GLenum>, ShaderUniformType>()();
};


template <size_t i, class... Args>
class _ShaderProgram {};

template <size_t i, class T>
class _ShaderProgram<i, T> : public _ShaderProgram<i - 1, T, T> {};

template <size_t i, class T, class... Args>
class _ShaderProgram<i, T, Args...> : public _ShaderProgram<i - 1, Args..., T, T> {};

template <class... Args>
class _ShaderProgram<0, Args...> {
public:
	static void load() {
		shader = std::make_unique<Shader>(initializeShader());
		LOG::SPGL::debug<&load, _ShaderProgram<0, Args...>>(std::vector<SQL::Types::TEXT>({ Tag::Class::ShaderProgram::LoadingProcedure }), "Shader loaded!");
	}

	static void use() {
		assert(shader != nullptr);
		use();
	}

public:
	inline static std::unique_ptr<Shader> shader = nullptr;

	template<size_t... n>
	static Shader _initializeShader(std::index_sequence<n...>) {
		LOG::SPGL::debug<&_initializeShader<n...>, _ShaderProgram<0, Args...>>(std::vector<SQL::Types::TEXT>({ Tag::Class::ShaderProgram::LoadingProcedure }),  "initializing shader with arguments: !!update log entry for viewing of arguments!!");
		return Shader((ShaderInfoFromType<n, Args>().value)...);
	}

	static Shader initializeShader() {
		LOG::SPGL::debug<&initializeShader, _ShaderProgram<0, Args...>>(std::vector<SQL::Types::TEXT>({ Tag::Class::ShaderProgram::LoadingProcedure }), 
			std::format("running shader initialization with {} as argument", typeid(decltype(std::make_index_sequence<sizeof...(Args)>{})).name()));
		return _initializeShader(std::make_index_sequence<sizeof...(Args)>{});
	}

	//duplicate of load() ???
	static void reloadShader() {
		LOG::SPGL::debug<&reloadShader, _ShaderProgram<0, Args...>>("Duplicate function, consider using load instead!");
		shader = std::make_unique<Shader>(initializeShader());
	}

	inline static auto FR = DY::FunctionRegister<
		//&_initializeShader<>,
		&load,
		&initializeShader,
		&reloadShader
	>(
		"load",
		"initializeShader",
		"reloadShader");

	inline static auto FB = DY::FunctionBinder(FR);

	using LOG = _LOG<DY::No_CB, DY::No_OB, decltype(FB), DY::No_VB>;
};

template <class... Args>
class ShaderProgram {
};

template <class... Args>
class ShaderProgram<const ShaderUniformPair<Args...>> : protected _ShaderProgram<sizeof...(Args), Args...> {
protected:
public:
	using Program = const ShaderUniformPair<Args...>;
public:
	static void load() {
		LOG::SPGL::debug<&load, ShaderProgram<const ShaderUniformPair<Args...>>>(std::vector<SQL::Types::TEXT>({ Tag::Class::ShaderProgram::LoadingProcedure }), "Loading shader...");
		_ShaderProgram<sizeof...(Args), Args...>::load();
	}

	static void use() {
		_ShaderProgram<sizeof...(Args), Args...>::use();
	}

	inline static auto FR = DY::FunctionRegister<&load>("load");

	inline static auto FB = DY::FunctionBinder(FR);

	using LOG = _LOG<DY::No_CB, DY::No_OB, decltype(FB), DY::No_VB>;
};


//template<class... Args>
//const Shader ShaderProgram<const ShaderUniformPair<Args...>>::shader{ShaderProgram<const ShaderUniformPair<Args...>>::initializeShader()};
