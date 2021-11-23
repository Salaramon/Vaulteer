#include "Texture.h"

Texture::Texture(std::string path, Binder::UniformInfo uniform) : uniform(uniform)
{
	debug("Loading texture: " + path + " for uniform: " + std::string(uniform.name) + "\n");
	initialize();
	loadTexture(path);
}

Texture::Texture(Binder::UniformInfo uniform, GLsizei width, GLsizei height, std::vector<glm::vec4> colors) : uniform(uniform), width(width), height(height)
{
	debug("Loading " + std::to_string(colors.size()) + "colors texture : " + " for uniform : " + std::string(uniform.name) + "\n");
	initialize();
	loadTexture(colors);
}

Texture::Texture(Texture&& other) : textureID(other.textureID)
{
	other.textureID = 0;
}

Texture::~Texture()
{
	cleanup();
}

void Texture::activate(const Shader& shader, GLint index) const
{
	shader.setUniform(uniform, index);
	glBindTextureUnit(index, textureID);
}


void Texture::initialize()
{
	glCreateTextures(GL_TEXTURE_2D, 1, &textureID);
}

void Texture::cleanup()
{
	glDeleteTextures(1, &textureID);
}

void Texture::loadTexture(std::string path) {
	
	uint8_t* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);

	if (data) {
		GLenum format;
		GLenum internalFormat;

		switch (nrComponents) {
		case 1:
			format = GL_RED;
			internalFormat = GL_R8I;
			break;
		case 3:
			format = GL_RGB;
			internalFormat = GL_RGB8;
			break;
		case 4:
			format = GL_RGBA;
			internalFormat = GL_RGBA8;
			break;
		}

		glGenerateTextureMipmap(textureID);

		glTextureStorage2D(textureID, 1, internalFormat, width, height);
		glTextureSubImage2D(textureID, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, data);

		setWrap(GL_REPEAT, GL_REPEAT);


		stbi_image_free(data);
	}
	else {
		debug("Failed to load texture: \n\t" + path + "\n");
	}
}

void Texture::loadTexture(std::vector<glm::vec4> colors)
{
	std::vector<unsigned char> pixelData;
	for (auto& color : colors) {
		pixelData.push_back(static_cast<unsigned char>(color.x * 255));
		pixelData.push_back(static_cast<unsigned char>(color.y * 255));
		pixelData.push_back(static_cast<unsigned char>(color.z * 255));
		pixelData.push_back(static_cast<unsigned char>(color.w * 255));
	}
	
	glTextureStorage2D(textureID, 1, GL_RGBA8, width, height);
	glTextureSubImage2D(textureID, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixelData.data());

	glGenerateTextureMipmap(textureID);
}

void Texture::setWrap(GLenum x, GLenum y) 
{
	glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, x);
	glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, y);
}

void Texture::setMinifyingFilter(GLenum filter) {
	glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, filter);
}

void Texture::setMagnifyingFilter(GLenum filter) {
	glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, filter);
}

void Texture::setAnisotropicFilter(GLfloat filter) {
	glTextureParameterf(textureID, GL_TEXTURE_MAX_ANISOTROPY, filter);
}