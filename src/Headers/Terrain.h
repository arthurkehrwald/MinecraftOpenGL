#pragma once
#include "Mesh.h"
#include "TextureAtlas.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <limits>

class Terrain
{
public:
	Terrain(const std::vector<std::vector<int>> & heightMap, const TextureAtlas & textureAtlas);
	void activate();
	void draw();
private:
	const std::vector<std::vector<int>> & heightMap;
	const TextureAtlas& textureAtlas;
	int snowHeight;
	int waterHeight;
	Mesh mesh;
	void addQuadStripToMesh(
		glm::vec3 pos,
		int goalHeight,
		bool isInXDirection,
		std::vector<float>& meshCoords,
		std::vector<int>& meshIndices);
	void addQuadToMesh(
		const glm::vec3& a,
		const glm::vec3& b,
		const glm::vec3& c,
		const glm::vec3& d,
		bool clockwise,
		const glm::vec3& normal,
		const glm::vec4& UVs,
		std::vector<float>& coords,
		std::vector<int>& indices);
};