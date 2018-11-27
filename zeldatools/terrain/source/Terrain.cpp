#include "Terrain.h"
#include <glad\glad.h>
#include <math.h>
#include "Color.h"
#include "HGHT.h"
#include "MATE.h"
#include "WATER.h"
#include "filenameUtils.h"

#include <fstream>
#include <iostream>
#include <iomanip>

#include <glm\gtc\constants.hpp>

Terrain::~Terrain()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
}

void Terrain::loadData(const std::string& filename, bool loadMaterial, bool loadWater)
{
	m_verts.clear();
	m_indices.clear();

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);

	// filename is just the stem
	m_hght.load(filenameUtils::getHGHTFolder() + "\\" + filename + ".hght");

	// create terrain vertex grid
	for (int y = 0, i = 0; y < 256; y++)
	{
		for (int x = 0; x < 256; x++, i++)
		{
			// add vert
			Vertex currentVert;
			currentVert.position = glm::vec4(x, 0, y, 1);

			currentVert.position.y = m_hght.getHeight(255 - x, y);

			currentVert.texcoord = glm::vec2((float)y / 255.0f, (float)(255 - x) / 255.0f);
			
			currentVert.isWater = 0;

			m_verts.push_back(currentVert);

			// add indices
			if (x < 255 && y < 255)
			{
				m_indices.push_back(i);
				m_indices.push_back(i + 256);
				m_indices.push_back(i + 257);

				m_indices.push_back(i);
				m_indices.push_back(i + 257);
				m_indices.push_back(i + 1);
			}
		}
	}

	// calculate normals for terrain
	for (unsigned int x = 0, i = 0; x < 256; x++)
	{
		for (unsigned int y = 0; y < 256; y++, i++)
		{
			if (x > 0 && x < 255 && y > 0 && y < 255)
			{
				m_verts[i].normal = glm::vec4(getTriangleNormal(i), 0.0f);
			}
			else
			{
				m_verts[i].normal = glm::vec4(0, 1, 0, 0);
			}
		}
	}

	if (loadMaterial)
	{
		m_mate.load(filenameUtils::getMATEFolder() + "\\" + filename + ".mate");

		for (int y = 0, i = 0; y < 256; y++)
		{
			for (int x = 0; x < 256; x++, i++)
			{
				MATEinfo currentMateInfo = m_mate.getMateInfo(255 - x, y);

				m_verts[i].MATEInfo.x = (float)currentMateInfo.id1 / 255.0f;
				m_verts[i].MATEInfo.y = (float)currentMateInfo.id2 / 255.0f;

				m_verts[i].unknownValue = (float)currentMateInfo.unk / 255.0f;

				m_verts[i].blendfac = (float)currentMateInfo.blend / 255.0f;
			}
		}
	}

	if (loadWater && fs::exists(fs::path(filenameUtils::getWATERFolder() + "\\" + filename + ".water.extm")))
	{
		m_water.load(filenameUtils::getWATERFolder() + "\\" + filename + ".water.extm");

		// create water vertex grid
		for (int y = 0, i = 65536; y < 64; y++)
		{
			for (int x = 0; x < 64; x++, i++)
			{
				// add vert
				Vertex currentVert;
				// multiply by 4.047619047619048 to match terrain size
				currentVert.position = glm::vec4(x * 4.047619047619048f, 0, y * 4.047619047619048f, 1);

				WATERInfo currentWaterInfo = m_water.getWaterInfo(63 - x, y);

				currentVert.position.y = currentWaterInfo.height;

				// just stuff the index into the mateinfo x
				currentVert.MATEInfo.x = (float)currentWaterInfo.materialIndex / 255.0f;

				currentVert.texcoord = glm::vec2((float)x / 63.0f, (float)y / 63.0f);

				currentVert.isWater = 1;

				m_verts.push_back(currentVert);

				// add indices
				if (x < 63 && y < 63)
				{
					m_indices.push_back(i);
					m_indices.push_back(i + 64);
					m_indices.push_back(i + 65);

					m_indices.push_back(i);
					m_indices.push_back(i + 65);
					m_indices.push_back(i + 1);
				}
			}
		}

		// calculate normals for water
		for (unsigned int x = 0, i = 65536; x < 64; x++)
		{
			for (unsigned int y = 0; y < 64; y++, i++)
			{
				if (x > 0 && x < 63 && y > 0 && y < 63)
				{
					m_verts[i].normal = glm::vec4(getTriangleNormal(i), 0.0f);
				}
				else
				{
					m_verts[i].normal = glm::vec4(0, 1, 0, 0);
				}
			}
		}
	}

	initialise();
}

void Terrain::initialise()
{
	assert(vao == 0);

	// generate buffers
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);

	// bind vertex array aka a mesh wrapper
	glBindVertexArray(vao);

	// bind vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// fill vertex buffer
	glBufferData(GL_ARRAY_BUFFER, m_verts.size() * sizeof(Vertex),
		&m_verts[0], GL_STATIC_DRAW);

	// enable first element as position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

	// enable second element as texCoord
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texcoord));

	// enable third element as normal
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

	// enable forth element as MATEInfo
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, MATEInfo));

	// enable fifth element as unknown value
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, unknownValue));

	// enable sixth element as blendfac
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, blendfac));

	// enable seventh element as isWater
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 1, GL_INT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, isWater));

	// bind indices if there are any
	if (m_indices.size() != 0)
	{
		glGenBuffers(1, &ibo);

		// bind vertex buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

		// fill vertex buffer
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);
	}

	// unbind buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Terrain::draw(Shader shader)
{
	glBindVertexArray(vao);

	// using indices or just vertices
	if (ibo != 0)
	{
		glDrawElements(GL_TRIANGLES, (GLsizei)m_indices.size(),
			GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, (GLsizei)m_verts.size());
	}
}

glm::vec3 Terrain::getTriangleDirection(glm::vec3 a, glm::vec3 b, glm::vec3 c)
{
	return glm::cross(b - a, c - a);
}

glm::vec3 Terrain::getTriangleNormal(int i)
{
	glm::vec3 n1 = getTriangleDirection(m_verts[i + 256].position, m_verts[i + 1].position, m_verts[i].position);
	glm::vec3 n2 = getTriangleDirection(m_verts[i - 256].position, m_verts[i - 1].position, m_verts[i].position);

	return glm::normalize(n1 + n2);
}
