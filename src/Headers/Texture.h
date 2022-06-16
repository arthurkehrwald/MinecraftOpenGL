#pragma once
#include "stb_image.h"

#include <glad/glad.h>

#include <iostream>

class Texture
{
public:
	unsigned int ID;

	Texture(const char* texturePath, GLenum format);

	void activate(int index);
};