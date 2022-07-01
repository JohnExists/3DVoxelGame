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
	void useSlot(const Shader& shader, 
		const std::string variableName, int slot);

	/**
	* Chooses how to wrap a texture in case
	* the specified tex parameters are too large
	* 
	* (ex. GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER)
	* 
	* @param x
	* Set the wrap on the x axis
	* 
	* @param y
	* Set the wrap on the y axis
	*/

	void setWrapAround(GLint x, GLint y, GLint z);

	/**
	* Chooses how to manipulate an image when
	* it scales up or down
	* 
	* (ex. GL_LINEAR, GL_NEAREST)
	* 
	* @param min
	* Chooses how to maniupulate a minimizing image 
	*
	* @param max
	* Chooses how to maniupulate a maximizing image
	*/
	void setScalingFilter(GLint min, GLint max);
	

private:
	void init();
	void load(const char* pathToTexture, GLenum textureTarget);

};
#endif // !TEXTURE_H



