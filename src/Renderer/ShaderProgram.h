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
using DeferredShader = const ShaderUniformPair<Binder::deferred_vertex, Binder::deferred_frag>;
using GeometryShader = const ShaderUniformPair<Binder::geometry_vertex, Binder::geometry_frag>;
using VolumeShader = const ShaderUniformPair<Binder::volume_vertex, Binder::volume_geom, Binder::volume_frag>;

using ClusterTileComputeShader = const ShaderUniformPair<Binder::cluster_tile_compute>;


template <class ShaderBinder>
constexpr const char* getShaderFile() {
	if constexpr (std::is_same_v<ShaderBinder, Binder::blending_vertex>) {
		return Binder::file_names::blending_vertex;
	}
	if constexpr (std::is_same_v<ShaderBinder, Binder::blending_frag>) {
		return Binder::file_names::blending_frag;
	}

	if constexpr (std::is_same_v<ShaderBinder, Binder::blending_composite_vertex>) {
		return Binder::file_names::blending_composite_vertex;
	}
	if constexpr (std::is_same_v<ShaderBinder, Binder::blending_composite_frag>) {
		return Binder::file_names::blending_composite_frag;
	}

	if constexpr (std::is_same_v<ShaderBinder, Binder::forward_vertex>) {
		return Binder::file_names::forward_vertex;
	}
	if constexpr (std::is_same_v<ShaderBinder, Binder::forward_frag>) {
		return Binder::file_names::forward_frag;
	}

	if constexpr (std::is_same_v<ShaderBinder, Binder::line_vertex>) {
		return Binder::file_names::line_vertex;
	}
	if constexpr (std::is_same_v<ShaderBinder, Binder::line_frag>) {
		return Binder::file_names::line_frag;
	}

	if constexpr (std::is_same_v<ShaderBinder, Binder::deferred_vertex>) {
		return Binder::file_names::deferred_vertex;
	}
	if constexpr (std::is_same_v<ShaderBinder, Binder::deferred_frag>) {
		return Binder::file_names::deferred_frag;
	}

	if constexpr (std::is_same_v<ShaderBinder, Binder::geometry_vertex>) {
		return Binder::file_names::geometry_vertex;
	}
	if constexpr (std::is_same_v<ShaderBinder, Binder::geometry_frag>) {
		return Binder::file_names::geometry_frag;
	}

	if constexpr (std::is_same_v<ShaderBinder, Binder::volume_vertex>) {
		return Binder::file_names::volume_vertex;
	}
	if constexpr (std::is_same_v<ShaderBinder, Binder::volume_geom>) {
		return Binder::file_names::volume_geom;
	}
	if constexpr (std::is_same_v<ShaderBinder, Binder::volume_frag>) {
		return Binder::file_names::volume_frag;
	}

	if constexpr (std::is_same_v<ShaderBinder, Binder::cluster_tile_compute>) {
		return Binder::file_names::cluster_tile_compute;
	}
}

template <class ShaderBinder>
constexpr const GLenum getShaderType() {
	if constexpr (std::is_same_v<ShaderBinder, Binder::blending_vertex>) {
		return GL_VERTEX_SHADER;
	}
	if constexpr (std::is_same_v<ShaderBinder, Binder::blending_frag>) {
		return GL_FRAGMENT_SHADER;
	}

	if constexpr (std::is_same_v<ShaderBinder, Binder::blending_composite_vertex>) {
		return GL_VERTEX_SHADER;
	}
	if constexpr (std::is_same_v<ShaderBinder, Binder::blending_composite_frag>) {
		return GL_FRAGMENT_SHADER;
	}

	if constexpr (std::is_same_v<ShaderBinder, Binder::forward_vertex>) {
		return GL_VERTEX_SHADER;
	}
	if constexpr (std::is_same_v<ShaderBinder, Binder::forward_frag>) {
		return GL_FRAGMENT_SHADER;
	}

	if constexpr (std::is_same_v<ShaderBinder, Binder::line_vertex>) {
		return GL_VERTEX_SHADER;
	}
	if constexpr (std::is_same_v<ShaderBinder, Binder::line_frag>) {
		return GL_FRAGMENT_SHADER;
	}

	if constexpr (std::is_same_v<ShaderBinder, Binder::deferred_vertex>) {
		return GL_VERTEX_SHADER;
	}
	if constexpr (std::is_same_v<ShaderBinder, Binder::deferred_frag>) {
		return GL_FRAGMENT_SHADER;
	}

	if constexpr (std::is_same_v<ShaderBinder, Binder::geometry_vertex>) {
		return GL_VERTEX_SHADER;
	}
	if constexpr (std::is_same_v<ShaderBinder, Binder::geometry_frag>) {
		return GL_FRAGMENT_SHADER;
	}

	if constexpr (std::is_same_v<ShaderBinder, Binder::volume_vertex>) {
		return GL_VERTEX_SHADER;
	}
	if constexpr (std::is_same_v<ShaderBinder, Binder::volume_geom>) {
		return GL_GEOMETRY_SHADER;
	}
	if constexpr (std::is_same_v<ShaderBinder, Binder::volume_frag>) {
		return GL_FRAGMENT_SHADER;
	}

	if constexpr (std::is_same_v<ShaderBinder, Binder::cluster_tile_compute>) {
		return GL_COMPUTE_SHADER;
	}
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
	}

	//protected:
	inline static std::unique_ptr<Shader> shader = nullptr;

	template <size_t... n>
	static Shader _initializeShader(std::index_sequence<n...>) {
		return Shader((ShaderInfoFromType<n, Args>().value)...);
	}

	static Shader initializeShader() {
		return _initializeShader(std::make_index_sequence<sizeof...(Args)>{});
	}

	static void reloadShader() {
		shader = std::make_unique<Shader>(initializeShader());
		//shader->populateUniformCache();
	}
};

template <class... Args>
class ShaderProgram {
};

template <class... Args>
class ShaderProgram<const ShaderUniformPair<Args...>> : protected _ShaderProgram<sizeof...(Args), Args...> {
protected:
	using Program = const ShaderUniformPair<Args...>;
public:
	static void load() {
		_ShaderProgram<sizeof...(Args), Args...>::load();
	}
};


//template<class... Args>
//const Shader ShaderProgram<const ShaderUniformPair<Args...>>::shader{ShaderProgram<const ShaderUniformPair<Args...>>::initializeShader()};
