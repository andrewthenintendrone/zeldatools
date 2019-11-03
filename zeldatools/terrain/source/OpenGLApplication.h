#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "FlyCamera.h"
#include "Light.h"
#include "Terrain.h"
#include "Color.h"
#include "Input.h"
#include "TextureHolder.h"

#include "Texture2D.h"

// OpenGLApplication class that manages everything
class OpenGLApplication
{
public:

	OpenGLApplication(unsigned int width = 1280, unsigned int height = 720, const char* windowTitle = "Open GL");
	~OpenGLApplication();

	void run();

	// mouse info
	glm::vec2 m_lastMousePos = glm::vec2(-1, -1);

	// Camera
	FlyCamera m_camera;

private:

	// these should not be called externally
	void setup();
	void update();
	void render();
	void processInput();
	void exit();

	// window width / height
	GLFWwindow* m_window = nullptr;
	unsigned int m_windowWidth;
	unsigned int m_windowHeight;

	// Shader(s)
	Shader m_terrainShader;

	// Light(s)
	std::vector<DirectionalLight> m_directionalLights;
	std::vector<PointLight> m_pointLights;

	// Terrain
	Terrain m_terrain;

	float m_textureScale;
	float m_heightScale;

	float rotation = 0;

	std::vector<std::string> m_filenames;
	int m_filenameIndex = 0;
};