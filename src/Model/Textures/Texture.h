#pragma once

#include <string>
#include <iostream>
#include <unordered_map>
#include <set>

#include <glad/glad.h>
#include <assimp/scene.h>

#include "stb_image.h"

#include "GLSLCPPBinder.h"
#include "Renderer/Shader.h"
#include "Utils/TypeDefUtils.h"

#include "Debug/Debug.h"

class Texture {
public:
    Texture();
    Texture(bool mipmapEnabled);
    Texture(GLsizei width, GLsizei height, bool mipmapEnabled = true);

    void setMinifyingFilter(GLenum filter);
    void setMagnifyingFilter(GLenum filter);
    void setAnisotropicFilter(GLfloat filter);

    GLuint textureID = 0;
    GLsizei width = -1, height = -1;
    GLint nrComponents = -1;
    bool mipmapEnabled = false;

    inline static std::unordered_map<aiTextureType, Binder::UniformInfo> uniformTextureTypes;
protected:
    void createTexture(GLenum type);
    void cleanup();

    std::pair<GLint, GLint> getFormatsFromComponents(int comp);


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