#include "Texture.h"

Texture::Texture(std::string path, Binder::UniformInfo uniform) : uniform(uniform)
{
	debug("Loading texture: " + path + " for uniform: " + std::string(uniform.name) + "\n");
	loadTexture(path);
}


void Texture::loadTexture(std::string path) {
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	uint8_t* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);

	if (data) {
		GLenum format{};

		switch (nrComponents) {
		case 1:
			format = GL_RED;
			break;
		case 3:
			format = GL_RGB;
			break;
		case 4:
			format = GL_RGBA;
			break;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else {
		debug("Failed to load texture: \n\t" + path + "\n");
	}
}