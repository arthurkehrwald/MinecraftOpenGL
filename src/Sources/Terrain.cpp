#include "Terrain.h"

Terrain::Terrain(const std::vector<std::vector<int>> & heightMap, const TextureAtlas & textureAtlas) : heightMap(heightMap), textureAtlas(textureAtlas)
{
	int maxHeight = std::numeric_limits<int>::min();
	int minHeight = std::numeric_limits<int>::max();

	for (int x = 0; x < heightMap.size(); x++)
	{
		for (int z = 0; z < heightMap[x].size(); z++)
		{
			int y = heightMap[x][z];

			if (y > maxHeight)
			{
				maxHeight = y;
			}
			if (y < minHeight)
			{
				minHeight = y;
			}
		}
	}

	int heightRange = abs(maxHeight) - abs(minHeight);

	if (heightRange < 4)
	{
		snowHeight = std::numeric_limits<int>::max();
		waterHeight = std::numeric_limits<int>::min();
	}
	else
	{
		snowHeight = minHeight + heightRange * 0.8f;
		waterHeight = minHeight + heightRange * 0.3f;
	}


	std::vector<float> meshCoords;
	std::vector<int> meshIndices;

	for (int x = 0; x < heightMap.size(); x++)
	{
		for (int z = 0; z < heightMap[x].size(); z++)
		{
			int y = heightMap[x][z];

			glm::vec3 a(x, y, z);
			glm::vec3 b(x + 1, y, z);
			glm::vec3 c(x, y, z + 1);
			glm::vec3 d(x + 1, y, z + 1);
			
			glm::vec3 normal(0.0f, 1.0f, 0.0f);

			TextureAtlas::SubTexture textureType;

			if (y > snowHeight)
			{
				textureType = TextureAtlas::SubTexture::snowTop;
			}
			else if (y > waterHeight)
			{
				textureType = TextureAtlas::SubTexture::dirtTop;
			}
			else
			{
				textureType = TextureAtlas::SubTexture::sand;
			}

			glm::vec4 UVs = textureAtlas.GetSubTextureCoords(textureType);
			addQuadToMesh(a, b, c, d, false, normal, UVs, meshCoords, meshIndices);

			if (x + 1 < heightMap.size())
			{
				addQuadStripToMesh(a, heightMap[x + 1][z], true, meshCoords, meshIndices);
			}

			if (z + 1 < heightMap[x].size())
			{
				addQuadStripToMesh(a, heightMap[x][z + 1], false, meshCoords, meshIndices);
			}

			if (y < waterHeight)
			{
				a.y = waterHeight;
				b.y = waterHeight;
				c.y = waterHeight;
				d.y = waterHeight;
				UVs = textureAtlas.GetSubTextureCoords(TextureAtlas::SubTexture::water);
				addQuadToMesh(a, b, c, d, false, normal, UVs, meshCoords, meshIndices);
			}
		}
	}

	mesh = Mesh(meshCoords, Mesh::VertexArrayFormat::posNormalUV, meshIndices);
}

void Terrain::activate()
{
	mesh.activate();
}

void Terrain::draw()
{
	mesh.draw();
}

void Terrain::addQuadStripToMesh(
	glm::vec3 pos,
	int adjacentHeight,
	bool isInXDirection,
	std::vector<float>& meshCoords,
	std::vector<int>& meshIndices)
{
	float bottomHeight;
	float topHeight;
	bool ascending = pos.y < adjacentHeight;

	if (ascending)
	{
		bottomHeight = pos.y;
		topHeight = adjacentHeight;
	}
	else
	{
		bottomHeight = adjacentHeight;
		topHeight = pos.y;
	}

	for (int y = bottomHeight; y < topHeight; y++)
	{
		glm::vec3 a, b, c, d;
		glm::vec3 normal;

		if (isInXDirection)
		{
			a = glm::vec3(pos.x + 1, y + 1, pos.z);
			b = glm::vec3(pos.x + 1, y + 1, pos.z + 1);
			c = glm::vec3(pos.x + 1, y, pos.z);
			d = glm::vec3(pos.x + 1, y, pos.z + 1);
			normal = glm::vec3(1.0f, 0.0f, 0.0f);
		}
		else
		{
			a = glm::vec3(pos.x + 1, y + 1, pos.z + 1);
			b = glm::vec3(pos.x, y + 1, pos.z + 1);
			c = glm::vec3(pos.x + 1, y, pos.z + 1);
			d = glm::vec3(pos.x, y, pos.z + 1);
			normal = glm::vec3(0.0f, 0.0f, 1.0f);
		}

		if (ascending)
		{
			normal = -normal;
		}

		TextureAtlas::SubTexture textureType;

		if (y >= snowHeight && y == topHeight - 1)
		{
			textureType = TextureAtlas::SubTexture::snowSideTop;
		}
		else if (y >= waterHeight)
		{
			if (y == topHeight - 1)
			{
				textureType = TextureAtlas::SubTexture::dirtSideTop;
			}
			else
			{
				textureType = TextureAtlas::SubTexture::dirt;
			}
		}
		else
		{
			textureType = TextureAtlas::SubTexture::sand;
		}

		glm::vec4 UVs = textureAtlas.GetSubTextureCoords(textureType);

		addQuadToMesh(a, b, c, d, !ascending, normal, UVs, meshCoords, meshIndices);
	}
}

void Terrain::addQuadToMesh(
	const glm::vec3& a,
	const glm::vec3& b,
	const glm::vec3& c,
	const glm::vec3& d,
	bool invertVertexOrder,
	const glm::vec3& normal,
	const glm::vec4& UVs,
	std::vector<float>& meshCoords,
	std::vector<int>& meshIndices)
{
	// A---B
	// |   |
	// C---D
	float quadCoords[]
	{
		a.x, a.y, a.z, normal.x, normal.y, normal.z, UVs.x, UVs.w,
		b.x, b.y, b.z, normal.x, normal.y, normal.z, UVs.z, UVs.w,
		c.x, c.y, c.z, normal.x, normal.y, normal.z, UVs.x, UVs.y,
		d.x, d.y, d.z, normal.x, normal.y, normal.z, UVs.z, UVs.y
	};

	for (const float quadCoord : quadCoords)
	{
		meshCoords.push_back(quadCoord);
	}

	const int valuesPerVertex = 8;
	const int vertexCount = meshCoords.size() / valuesPerVertex;

	// A, C, B
	// B, C, D
	float quadIndices[]
	{
		vertexCount - 4, vertexCount - 2, vertexCount - 3,
		vertexCount - 3, vertexCount - 2, vertexCount - 1
	};

	if (invertVertexOrder)
	{
		for (int i = 5; i >= 0; i--)
		{
			meshIndices.push_back(quadIndices[i]);
		}
	}
	else
	{
		for (int i = 0; i <= 5; i++)
		{
			meshIndices.push_back(quadIndices[i]);
		}
	}
}