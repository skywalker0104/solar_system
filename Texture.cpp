#include <stb/stb_image.h>

#include "Texture.h"

#include <iostream>

void Texture::setTextureWrappingParam(GLenum S, GLenum T)
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, S);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, T);
}

void Texture::setTextureFilteringParam(GLenum Min, GLenum Mag)
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Mag);
}

void Texture::loadTexture()
{
	int width, height, n;
	unsigned char* image = stbi_load(imgPath.c_str(), &width, &height, &n, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(image);

	isLoaded = true;
}

void Texture::fastLoad()
{
	glGenTextures(1, &textureID);

	glBindTexture(GL_TEXTURE_2D, textureID);
	setTextureWrappingParam();
	setTextureFilteringParam();
	loadTexture();
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::useTexture()
{
	if (!isLoaded)
	{
		std::cerr << "ERROR::TEXTURE::This texture isn`t loaded!" << std::endl;
		return;
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::clearTexture()
{
	glDeleteTextures(1, &textureID);
	textureID = 0;
	isLoaded = false;
	imgPath = "";
}

Texture::~Texture()
{
	clearTexture();
}