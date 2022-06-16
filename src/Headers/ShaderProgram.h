#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H
#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class ShaderProgram
{
public:
	unsigned int ID;

	ShaderProgram(const char* vertShaderPath, const char* fragShaderPath);

	void activate();
	const void setUniform(const std::string& name, bool value);
	const void setUniform(const std::string& name, int value);
	const void setUniform(const std::string& name, float value);
	const void setUniform(const std::string& name, glm::mat4 value);
	const void setUniform(const std::string& name, glm::vec2 value);
	const void setUniform(const std::string& name, glm::vec3 value);
	const void setUniform(const std::string& name, glm::vec4 value);

private:
	const std::string const readShader(const char* path);
	const unsigned int compileShader(const char* src, int type);
};
#endif