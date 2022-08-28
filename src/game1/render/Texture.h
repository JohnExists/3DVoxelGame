#ifndef TEXTURE_H
#define TEXTURE_H

#include<glad/glad.h>
#include<array>
#include<stb/stb_image.h>
#include<string>
#include<iostream>
#include<cassert>
#include<filesystem>

#include "Shader.h"
/**
* 
* Loads images from disk and uses them in OpenGL
*
*/
class Texture
{
private:
	const unsigned int TEXTURE_LOADING_FAILURE = 0;

	GLuint texture{};
	GLenum target{};
	unsigned char* textureData{};
	int width{};
	int height{};
	int numberOfColorChannels{};

	GLint wrapAround{};
	GLint scalingFilter{};

public:
	/**
	* @param pathToTexture
	* Specify the filepath to the image you
	* want to load
	* 
	*/
	Texture(const char* pathToTexture);
	Texture() = default;

	/**
	 * Binds this texture to the current OpenGL context
	 */
	void use() const;

	/**
	 * Uses uniforms sampler2D objects 
	 * to move textures to the fragment shader
	 * 
	 * @param shader
	 * The shader object that contains 
	 * the fragment shader
	 * 
	 * @param variableName: 
	 * The name of the sampler2D 
	 * object in the fragment shader
	 *
	 * @param slot: 
	 * The slot in memory that is going to 
	 * hold the image
	 */
	void useSlot(
	Shader& shader, 
	std::string variableName, 
	int slot
	) const;

	/**
	* Chooses how to wrap a texture in case
	* the specified UV parameters are too large
	* 
	* (ex. GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER)
	* 
	* @param value
	* Value to set wrap around for x, y and z
	*/

	void setWrapAround(GLint value);

	/**
	* Chooses how to manipulate an image when
	* it scales up or down
	* 
	* (ex. GL_LINEAR, GL_NEAREST)
	* 
	* @param value
	* Chooses how to maniupulate a minimizing and maximizing image
	*/
	void setScalingFilter(GLint value);
	

private:
	void init();
	void load(const char* pathToTexture, GLenum textureTarget);

};
#endif // !TEXTURE_H



