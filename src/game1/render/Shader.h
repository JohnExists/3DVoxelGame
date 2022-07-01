#ifndef SHADER_H
#define SHADER_H

#include<glad/glad.h>

#include<glm/glm.hpp>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<type_traits>

class Shader
{
private:
	GLuint ID;

public:
	Shader(const char* vertexPath, const char* fragmentPath, int filler);
	Shader(const char* vertexPath, const char* fragmentPath);
	Shader() = default;

	void use() const;

	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setMatrix(const std::string& name, glm::mat4& matrix) const;
	void setVec3(const std::string& name, float x, float y, float z) const;

	GLuint getID() const;

private:
	void readFile(const char* filePath, std::string& outputString);
	void compile(const GLenum type, const char* shaderCode, GLuint& shader);
	void checkErrors(GLuint data, const std::string& name);
};

#endif // !SHADER_H
