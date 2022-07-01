#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>

//////////////////////////////////
/*		Public Functions		*/
//////////////////////////////////

Shader::Shader(const char* vertexCode, const char* fragmentCode)
{
	GLuint vertex{}, fragment{};

	ID = glCreateProgram();
	compile(GL_VERTEX_SHADER, vertexCode, vertex);
	compile(GL_FRAGMENT_SHADER, fragmentCode, fragment);

	glLinkProgram(ID);

	checkErrors(ID, "Program");
	glDeleteProgram(vertex);
	glDeleteProgram(fragment);
}

void Shader::use() const
{
	glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(
		glGetUniformLocation(ID, name.c_str()),
		static_cast<int>(value)
	);
}
void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setMatrix(const std::string& name, glm::mat4& matrix) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setVec3(const std::string& name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

GLuint Shader::getID() const
{
	return ID;
}

//////////////////////////////////
/*		Private Functions		*/
//////////////////////////////////

void Shader::compile(const GLenum type, const char* shaderCode,
	GLuint& shader)
{
	shader = glCreateShader(type);
	glShaderSource(shader, 1, &shaderCode, NULL);
	glCompileShader(shader);
	glAttachShader(ID, shader);
	checkErrors(shader, ((type == GL_VERTEX_SHADER) ? "Vertex" : "Fragment"));
}

void Shader::checkErrors(const GLuint data, const std::string& type)
{
	int success;
	char infoLog[1024];
	if (type == "Program")
	{
		glGetShaderiv(data, GL_LINK_STATUS, &success);
		if (success) return;
		glGetProgramInfoLog(data, 1024, NULL, infoLog);
	}
	else
	{
		glGetShaderiv(data, GL_COMPILE_STATUS, &success);
		if (success) return;
		glGetShaderInfoLog(data, 1024, NULL, infoLog);
	}

	std::cout << type << " failed to compile!\n" << infoLog << '\n';
}

