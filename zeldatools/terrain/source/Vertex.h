#pragma once
#include <glm\glm.hpp>

struct Vertex
{
	glm::vec4 position = glm::vec4(0, 0, 0, 1); // vertex position
	glm::vec2 texcoord = glm::vec2(0.5f, 0.5f); // vertex UV coordinates
	glm::vec4 normal = glm::vec4(0); // vertex normal
	glm::vec2 MATEInfo = glm::vec2(0, 0); // MATEInfo of vertex
	float unknownValue = 0; // unknown value of this vertex
	float blendfac = 0; // blending factor of vertex
	int isWater = 0; // is this vertex water
};