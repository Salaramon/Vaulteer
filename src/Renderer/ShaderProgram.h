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
using DeferredPointShader = const ShaderUniformPair<Binder::deferred_point_vertex, Binder::deferred_point_frag>;
using DeferredDirShader = const ShaderUniformPair<Binder::deferred_directional_vertex, Binder::deferred_directional_frag>;
using GeometryShader = const ShaderUniformPair<Binder::geometry_vertex, Binder::geometry_frag>;
using VolumeShader = const ShaderUniformPair<Binder::volume_vertex, Binder::volume_geom, Binder::volume_frag>;

using ClusterTileComputeShader = const ShaderUniformPair<Binder::cluster_tile_compute>;

class ShaderProgram {
public:
	#define files Binder::file_names


	static Shader& forwardShader() { 
		static Shader s = Shader(files::forward_vertex, GL_VERTEX_SHADER, files::forward_frag, GL_FRAGMENT_SHADER);
		return s;
	}

	static Shader& blendingShader() { 
		static Shader s = Shader(files::blending_vertex, GL_VERTEX_SHADER, files::blending_frag, GL_FRAGMENT_SHADER);
		return s;
	}
	static Shader& blendingCompositeShader() { 
		static Shader s = Shader(files::blending_composite_vertex, GL_VERTEX_SHADER, files::blending_composite_frag, GL_FRAGMENT_SHADER);
		return s;
	}
	static Shader& geometryShader() { 
		static Shader s = Shader(files::geometry_vertex, GL_VERTEX_SHADER, files::geometry_frag, GL_FRAGMENT_SHADER);
		return s;
	}
	static Shader& deferredPointShader() { 
		static Shader s = Shader(files::deferred_point_vertex, GL_VERTEX_SHADER, files::deferred_point_frag, GL_FRAGMENT_SHADER);
		return s;
	}
	static Shader& deferredDirShader() { 
		static Shader s = Shader(files::deferred_directional_vertex, GL_VERTEX_SHADER, files::deferred_directional_frag, GL_FRAGMENT_SHADER);
		return s;
	}

	static Shader& shadowVolumeShader() { 
		static Shader s = Shader(files::volume_vertex, GL_VERTEX_SHADER, files::volume_geom, GL_GEOMETRY_SHADER, files::volume_frag, GL_FRAGMENT_SHADER);
		return s;
	}

	#undef files
};


//template<class... Args>
//const Shader ShaderProgram<const ShaderUniformPair<Args...>>::shader{ShaderProgram<const ShaderUniformPair<Args...>>::initializeShader()};
