#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(const char* vertShaderPath, const char* fragShaderPath)
{
	ID = glCreateProgram();

	std::string vertShaderSrc = readShader(vertShaderPath);
	unsigned int vertShader = compileShader(vertShaderSrc.c_str(), GL_VERTEX_SHADER);
	glAttachShader(ID, vertShader);

	std::string fragShaderSrc = readShader(fragShaderPath);
	unsigned int fragShader = compileShader(fragShaderSrc.c_str(), GL_FRAGMENT_SHADER);
	glAttachShader(ID, fragShader);

	glLinkProgram(ID);
	int success;
	char infoLog[512];
	// print linking errors if any
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertShader);
	glDeleteShader(fragShader);
}

void ShaderProgram::activate()
{
	glUseProgram(ID);
}

const void ShaderProgram::setUniform(const std::string& name, bool value)
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

const void ShaderProgram::setUniform(const std::string& name, int value)
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

const void ShaderProgram::setUniform(const std::string& name, float value)
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

const void ShaderProgram::setUniform(const std::string& name, glm::mat4 value)
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

const void ShaderProgram::setUniform(const std::string& name, glm::vec2 value)
{
	glUniform2f(glGetUniformLocation(ID, name.c_str()), value.x, value.y);
}

const void ShaderProgram::setUniform(const std::string& name, glm::vec3 value)
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z);
}

const void ShaderProgram::setUniform(const std::string& name, glm::vec4 value)
{
	glUniform4f(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z, value.w);
}

const std::string const ShaderProgram::readShader(const char* path)
{
	std::string vertShaderSrcStr;
	std::ifstream vertShaderFile;
	// ensure ifstream objects can throw exceptions:
	vertShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		// open files
		vertShaderFile.open(path);
		// read file contents into streams
		std::stringstream vertShaderStream;
		std::stringstream fragShaderStream;
		vertShaderStream << vertShaderFile.rdbuf();
		// close file handlers
		vertShaderFile.close();
		// convert stream into string
		vertShaderSrcStr = vertShaderStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR:SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
	}
	return vertShaderSrcStr;
}

const unsigned int ShaderProgram::compileShader(const char* src, int type)
{
	unsigned int shader;
	int success;
	char infoLog[512];

	shader = glCreateShader(type);
	glShaderSource(shader, 1, &src, NULL);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	return shader;
}