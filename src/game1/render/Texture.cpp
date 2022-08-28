#include "Texture.h"

//////////////////////////////////
/*		Public Functions		*/
//////////////////////////////////

Texture::Texture(const char* pathToTexture)
{
	setScalingFilter(GL_NEAREST);
	setWrapAround(GL_CLAMP_TO_EDGE);

	target = GL_TEXTURE_2D;
	init();
	try
	{
		load(pathToTexture, target);
	}
	catch (const std::string& e)
	{
		std::cerr << "(ERROR WHEN LOADING " <<
			pathToTexture << ") " << e << '\n';
	}
}

void Texture::use() const
{
	if (texture == TEXTURE_LOADING_FAILURE) return;

	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, scalingFilter);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, scalingFilter);
	glTexParameteri(target, GL_TEXTURE_WRAP_S, wrapAround);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, wrapAround);
	glTexParameteri(target, GL_TEXTURE_WRAP_R, wrapAround);

	glBindTexture(target, texture);
}

void Texture::useSlot(Shader& shader, std::string variableName, int slot) const
{
	if (texture == TEXTURE_LOADING_FAILURE) return;
	assert((slot < 31 || slot > 0) && "Assign a slot between 0 and 31");

	shader[variableName] = slot;

	glActiveTexture(GL_TEXTURE0 + slot);
	use();
}

void Texture::setWrapAround(const GLint value = GL_REPEAT)
{
	wrapAround = value;
}

void Texture::setScalingFilter(const GLint value = GL_LINEAR) 
{
	scalingFilter = value;
}

//////////////////////////////////
/*		Private Functions		*/
//////////////////////////////////

void Texture::init()
{
	glGenTextures(1, &texture);
	use();
	setWrapAround();
	setScalingFilter();

	stbi_set_flip_vertically_on_load(true);
}

void Texture::load(const char* pathToTexture, GLenum textureTarget)
{
	textureData = stbi_load(pathToTexture,
		&width,
		&height,
		&numberOfColorChannels,
		0);

	if (!textureData) throw std::string("Texture file path does not exist!");

	int colorChannelFormat = {
			numberOfColorChannels > 3 ? GL_RGBA : GL_RGB
	};

	glTexImage2D(
		textureTarget,
		0,
		colorChannelFormat,
		width,
		height,
		0,
		colorChannelFormat,
		GL_UNSIGNED_BYTE,
		textureData
	);
	glGenerateMipmap(textureTarget);

	stbi_image_free(textureData);
}