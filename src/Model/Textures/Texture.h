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
};