#include "TextureAtlas.h"

TextureAtlas::TextureAtlas(const char* texturePath, GLenum format, const std::map<SubTexture, glm::vec4> & subTextureCoords) : Texture(texturePath, format), subTextureCoords(subTextureCoords)
{

}

const glm::vec4& TextureAtlas::GetSubTextureCoords(const SubTexture textureType) const
{
	if (subTextureCoords.find(textureType) == subTextureCoords.end())
	{
		return glm::vec4();
	}
	else
	{
		return subTextureCoords.at(textureType);
	}
}