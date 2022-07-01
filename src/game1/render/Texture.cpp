#include "Texture.h"

//////////////////////////////////
/*		Public Functions		*/
//////////////////////////////////

Texture::Texture(const char* pathToTexture)
{
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
	glBindTexture(target, texture);
}

void Texture::useSlot(
	const Shader& shader, 
	const std::string variableName, 
	int slot
)
{
	if (texture == TEXTURE_LOADING_FAILURE) return;
	assert((slot < 31 || slot > 0) && "Assign a slot between 0 and 31");

	shader.use();
	shader.setInt(variableName, slot);
	glActiveTexture(GL_TEXTURE0 + slot);
	use();
}

void Texture::setWrapAround(
	const GLint x = GL_REPEAT,
	const GLint y = GL_REPEAT,
	const GLint z = GL_REPEAT
)
{
	glTexParameteri(target, GL_TEXTURE_WRAP_S, x);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, y);
	glTexParameteri(target, GL_TEXTURE_WRAP_R, z);
}

void Texture::setScalingFilter(
	const GLint min = GL_LINEAR,
	const GLint max = GL_LINEAR
) 
{
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, min);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, max);
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