#ifndef SHADER_H
#define SHADER_H

#include<glad/glad.h>

#include<glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<type_traits>

#define FRAGMENT_PATH(fileName) "../res/shaders/" + fileName + ".frag"
#define VERTEX_PATH(fileName) "../res/shaders/" + fileName + ".vert"

class Shader
{
private:
	class ShaderUniform
	{
	private:
		GLuint value;
	public:
		ShaderUniform(GLint value);
		void operator=(glm::mat4 variableName);
		void operator=(glm::vec3 variableName);
		void operator=(float variableName);
		void operator=(int variableName);

	};

	GLint ID;

public:
	Shader(const char* vertexPath, const char* fragmentPath, int filler);
	Shader(const char* vertexPath, const char* fragmentPath);
	Shader(std::string shaderName);
	Shader() = default;

	void use() const;
	ShaderUniform operator[](std::string variableName);
	GLuint getID() const;

private:
	void load(const char* vertexPath, const char* fragmentPath);

	std::string readFile(const std::string filePath);
	void compile(const GLenum type, const char* shaderCode, GLuint& shader);
	void checkErrors(GLuint data, const std::string& name);
};



#endif // !SHADER_H
