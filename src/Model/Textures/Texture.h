#pragma once

#include <unordered_map>

#include <glad/glad.h>
#include <assimp/scene.h>

// ReSharper disable once CppUnusedIncludeDirective
#include "stb_image.h"

#include "GLSLCPPBinder.h"
#include "Renderer/Shader.h"

#include "Debug/Debug.h"

class Texture {
public:
	Texture();
	Texture(bool mipmapEnabled);
	Texture(GLsizei width, GLsizei height, bool mipmapEnabled = true);

	void setMinifyingFilter(GLenum filter) const;
	void setMagnifyingFilter(GLenum filter) const;
	void setAnisotropicFilter(GLfloat filter) const;

	GLuint textureID = 0;
	GLsizei width = -1, height = -1;
	GLint nrComponents = -1;
	bool mipmapEnabled = false;

	inline static std::unordered_map<aiTextureType, Binder::UniformInfo> uniformTextureTypes;
protected:
	void createTexture(GLenum type);
	void cleanup() const;
    
	static std::pair<GLint, GLint> getFormatsFromComponents(int nrComponents);

public:
    inline static auto CR = DY::ClassRegister<
        &setMinifyingFilter,
        &setMagnifyingFilter,
        &setAnisotropicFilter,
        &createTexture,
        &cleanup>();
    inline static auto CB = DY::ClassBinder<decltype(CR)>();

    DY::ObjectRegister<Texture,
        decltype(textureID),
        decltype(width),
        decltype(height),
        decltype(nrComponents),
        decltype(mipmapEnabled),
        decltype(uniformTextureTypes)> OR;
    inline static auto OB = DY::ObjectBinder<decltype(OR)>();
    
    using LOG = _LOG<decltype(CB), decltype(OB), DY::No_FB, DY::No_VB>;
};
