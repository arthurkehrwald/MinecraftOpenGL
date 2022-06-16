#pragma once
#include "Texture.h"
#include <map>
#include <glm/glm.hpp>

class TextureAtlas : public Texture
{
public:
	static const enum class SubTexture { dirtTop, dirtSideTop, dirt, snowTop, snowSideTop, sand, water};
	TextureAtlas(const char* texturePath, GLenum format, const std::map<SubTexture, glm::vec4> & subTextureCoords);
	const glm::vec4& GetSubTextureCoords(const SubTexture textureType) const;
private:
	const std::map<SubTexture, glm::vec4> subTextureCoords;
};