#include "Shader.h"

//////////////////////////////////
/*		Public Functions		*/
//////////////////////////////////

Shader::Shader(const char* vertexCode, const char* fragmentCode)
{
	load(vertexCode, fragmentCode);
}

Shader::Shader(std::string shaderName)
{
	std::string fragmentSource = readFile(FRAGMENT_PATH(shaderName));
	std::string vertexSource = readFile(VERTEX_PATH(shaderName));
	load(vertexSource.c_str(), fragmentSource.c_str());
}

void Shader::use() const
{
	glUseProgram(ID);
}

Shader::ShaderUniform Shader::operator[](std::string variableName)
{
	return ShaderUniform(glGetUniformLocation(ID, variableName.c_str()));
}

Shader::ShaderUniform::ShaderUniform(GLint value) : value(value) { }

void Shader::ShaderUniform::operator=(glm::mat4& variableValue)
{
	glUniformMatrix4fv(value, 1, GL_FALSE, glm::value_ptr(variableValue));
}

void Shader::ShaderUniform::operator=(glm::vec3 variableValue)
{
	glUniform3f(value, variableValue.x, variableValue.y, variableValue.z);
}

void Shader::ShaderUniform::operator=(float variableValue)
{
	glUniform1f(value, variableValue);
}

void Shader::ShaderUniform::operator=(int variableName)
{
	glUniform1i(value, variableName);
}

GLuint Shader::getID() const
{
	return ID;
}

void Shader::load(const char* vertexCode, const char* fragmentCode)
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

std::string Shader::readFile(const std::string filePath)
{
	std::ifstream file(filePath);
	if(!file.is_open()) std::cout << "File Opening Failure";
	std::stringstream buffer;
	buffer << file.rdbuf();

	return buffer.str();
}

//////////////////////////////////
/*		Private Functions		*/
//////////////////////////////////

void Shader::compile(const GLenum type, const char* shaderCode, GLuint& shader)
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