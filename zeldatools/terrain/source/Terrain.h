#pragma once
#include "Vertex.h"
#include "Shader.h"
#include <vector>
#include "HGHT.h"
#include "MATE.h"
#include "WATER.h"

class Terrain
{
public:

	Terrain() {}
	virtual ~Terrain();

	void loadData(const std::string& filename, bool loadMaterial = true, bool loadWater = true);

	void initialise();

	virtual void draw(Shader shader);

	const unsigned int getVertexArrayObject() { return vao; }
	const unsigned int getVertexBufferObject() { return vbo; }
	const unsigned int getIndexBufferObject() { return ibo; }

private:

	HGHT m_hght;
	MATE m_mate;
	WATER m_water;

	glm::vec3 getTriangleDirection(glm::vec3 a, glm::vec3 b, glm::vec3 c);
	glm::vec3 getTriangleNormal(int i);

	unsigned int vao = 0;
	unsigned int vbo = 0;
	unsigned int ibo = 0;
	std::vector<Vertex> m_verts;
	std::vector<unsigned int> m_indices;
};