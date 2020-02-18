#include "OpenGLApplication.h"

#include <glm\gtc\matrix_inverse.hpp>

#include "filenameUtils.h"
#include <iostream>

#include "Time.h"
#include "Color.h"

// callback functions
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

OpenGLApplication::OpenGLApplication(unsigned int width, unsigned int height, const char* windowTitle) : m_windowWidth(width), m_windowHeight(height)
{
	// initialise glfw
	glfwInit();

	// tell glfw we will be using OpenGL version 4.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create window using glfw
	m_window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);

	// check that the window was created sucessfully
	if (m_window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(m_window);

	// store a pointer to this instance of OpenGLApplication within the window
	glfwSetWindowUserPointer(m_window, this);

	// set up the various callback functions
	glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
	glfwSetCursorPosCallback(m_window, mouse_callback);
	glfwSetScrollCallback(m_window, scroll_callback);

	// tell GLFW to lock and hide the cursor
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// initialise glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}

	// enable the z buffer
	glEnable(GL_DEPTH_TEST);

	// enable front face culling
	glEnable(GL_CULL_FACE);

	Input::getInstance().setWindowPointer(m_window);

	// move on to setup
	setup();
}

OpenGLApplication::~OpenGLApplication()
{

}

// load and create all the various assets needed
void OpenGLApplication::setup()
{
	// load and compile shaders
	m_terrainShader = Shader((fs::current_path().string() + "\\resources\\shaders\\terrainShader.vs").c_str(),
		(fs::current_path().string() + "\\resources\\shaders\\terrainShader.fs").c_str());

	m_terrainShader.bind();

	m_terrainShader.setVec4("WaterColors[0]", Color::Aqua().asVec4());
	m_terrainShader.setVec4("WaterColors[1]", Color::Aquamarine().asVec4());
	m_terrainShader.setVec4("WaterColors[2]", Color::Purple().asVec4());
	m_terrainShader.setVec4("WaterColors[3]", Color::Red().asVec4());
	m_terrainShader.setVec4("WaterColors[4]", Color::Cyan().asVec4());
	m_terrainShader.setVec4("WaterColors[5]", Color::Brown().asVec4());
	m_terrainShader.setVec4("WaterColors[6]", Color::White().asVec4());
	m_terrainShader.setVec4("WaterColors[7]", Color::LightSeaGreen().asVec4());

	for (auto& iter : fs::directory_iterator(filenameUtils::getHGHTFolder()))
	{
		m_filenames.push_back(iter.path().stem().string());
	}

	m_terrain.loadData(m_filenames[m_filenameIndex], true, true);

	// set up light(s)
	DirectionalLight dLight;

	dLight.ambient = glm::vec3(1.0f);
	dLight.diffuse = glm::vec3(1.0f);
	dLight.specular = glm::vec3(1.0f);
	dLight.direction = glm::normalize(glm::vec3(0.612372f, -0.612372f, -0.5f));

	m_directionalLights.push_back(dLight);

	// set camera position
	m_camera.setPosition(glm::vec3(0, 15, 25));
	m_camera.setLookAt(glm::vec3(0, 15, 0));

	// create TextureHolder instance
	TextureHolder::getInstance();

	m_textureScale = 1.0f;
	m_heightScale = 1.0f / 4096.0f;
}

void OpenGLApplication::run()
{
	// continue to update and render while the window is still open
	while (!glfwWindowShouldClose(m_window))
	{
		update();
		render();
	}

	// clean up and exit
	exit();
}

void OpenGLApplication::update()
{
	// update Time
	Time::getInstance().update();

	// process input
	processInput();
}

void OpenGLApplication::render()
{
	// clear the color and depth buffers
	glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// bind shader
	m_terrainShader.bind();

	m_terrainShader.setVec3("cameraPosition", m_camera.getPosition());

	// bind terrain / water textures
	TextureHolder::getInstance().bindTextures(m_terrainShader);

	// set light
	m_terrainShader.setVec3("lightDir", m_directionalLights[0].direction);

	// draw terrain
	glm::mat4 model(1);
	model = glm::scale(model, glm::vec3(1.0f, m_heightScale, 1.0f));

	m_terrainShader.setMat4("ModelMatrix", model);
	glm::mat4 normalMatrix = glm::inverseTranspose(model);
	m_terrainShader.setMat4("NormalMatrix", normalMatrix);

	m_terrainShader.setMat4("ProjectionViewModel", m_camera.getProjectionViewMatrix() * model);

	// set texture scale
	m_terrainShader.setFloat("textureScale", m_textureScale);

	// set rotation
	m_terrainShader.setFloat("testValue", rotation);

	// draw water?
	m_terrainShader.setBool("drawWater", drawWater);

	m_terrain.draw(m_terrainShader);

	// swap buffers and poll window events
	glfwSwapBuffers(m_window);
	glfwPollEvents();
}

// handle any input that has occured
void OpenGLApplication::processInput()
{
	// update Input
	Input::getInstance().update();

	// escape exits
	if (Input::getInstance().getPressed(GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(m_window, true);

	// left shift enables faster camera movement
	if (Input::getInstance().getHeld(GLFW_KEY_LEFT_SHIFT))
	 m_camera.m_running = true;
	 else
	 m_camera.m_running = false;

	// move camera with WASD / arrow keys
	if (Input::getInstance().getHeld(GLFW_KEY_W) || Input::getInstance().getHeld(GLFW_KEY_UP))
		m_camera.processKeyboard(FORWARD);
	if (Input::getInstance().getHeld(GLFW_KEY_S) || Input::getInstance().getHeld(GLFW_KEY_DOWN))
		m_camera.processKeyboard(BACKWARD);
	if (Input::getInstance().getHeld(GLFW_KEY_A) || Input::getInstance().getHeld(GLFW_KEY_LEFT))
		m_camera.processKeyboard(LEFT);
	if (Input::getInstance().getHeld(GLFW_KEY_D) || Input::getInstance().getHeld(GLFW_KEY_RIGHT))
		m_camera.processKeyboard(RIGHT);

	// O / P decrease / increase texture scale
	if (Input::getInstance().getPressed(GLFW_KEY_O))
	{
		m_textureScale *= 0.5f;
	}
	if (Input::getInstance().getPressed(GLFW_KEY_P))
	{
		m_textureScale *= 2.0f;
	}

	// plus / minus decrease / increase texture rotation
	if (Input::getInstance().getPressed(GLFW_KEY_KP_ADD))
	{
		rotation += 5.0f;
	}
	if (Input::getInstance().getPressed(GLFW_KEY_KP_SUBTRACT))
	{
		rotation -= 5.0f;
	}

	// K / L decrease / increase height scale
	if (Input::getInstance().getPressed(GLFW_KEY_K))
	{
		m_heightScale *= 0.5f;
	}
	if (Input::getInstance().getPressed(GLFW_KEY_L))
	{
		m_heightScale *= 2.0f;
	}

	// 1 / 2 load new files (skip 1)
	if (Input::getInstance().getPressed(GLFW_KEY_1))
	{
		m_filenameIndex = (m_filenameIndex - 1) % m_filenames.size();
		m_terrain.loadData(m_filenames[m_filenameIndex], true, true);
	}
	if (Input::getInstance().getPressed(GLFW_KEY_2))
	{
		m_filenameIndex = (m_filenameIndex + 1) % m_filenames.size();
		m_terrain.loadData(m_filenames[m_filenameIndex], true, true);
	}

	// 3 / 4 load new files (skip 10)
	if (Input::getInstance().getPressed(GLFW_KEY_3))
	{
		m_filenameIndex = (m_filenameIndex - 10) % m_filenames.size();
		m_terrain.loadData(m_filenames[m_filenameIndex], true, true);
	}
	if (Input::getInstance().getPressed(GLFW_KEY_4))
	{
		m_filenameIndex = (m_filenameIndex + 10) % m_filenames.size();
		m_terrain.loadData(m_filenames[m_filenameIndex], true, true);
	}

	// 5 / 6 load new files (skip 100)
	if (Input::getInstance().getPressed(GLFW_KEY_5))
	{
		m_filenameIndex = (m_filenameIndex - 100) % m_filenames.size();
		m_terrain.loadData(m_filenames[m_filenameIndex], true, true);
	}
	if (Input::getInstance().getPressed(GLFW_KEY_6))
	{
		m_filenameIndex = (m_filenameIndex + 100) % m_filenames.size();
		m_terrain.loadData(m_filenames[m_filenameIndex], true, true);
	}

	// 7 / 8 load new files (skip 1000)
	if (Input::getInstance().getPressed(GLFW_KEY_7))
	{
		m_filenameIndex = (m_filenameIndex - 1000) % m_filenames.size();
		m_terrain.loadData(m_filenames[m_filenameIndex], true, true);
	}
	if (Input::getInstance().getPressed(GLFW_KEY_8))
	{
		m_filenameIndex = (m_filenameIndex + 1000) % m_filenames.size();
		m_terrain.loadData(m_filenames[m_filenameIndex], true, true);
	}

	// 9 jumps to specific file
	if (Input::getInstance().getPressed(GLFW_KEY_9))
	{
		m_terrain.loadData("580000C7BC", true, true);
	}

	// M toggles drawing of water
	if (Input::getInstance().getPressed(GLFW_KEY_M))
	{
		drawWater = !drawWater;
	}

	// draw in wireframe if space is held
	if (Input::getInstance().getHeld(GLFW_KEY_SPACE))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void OpenGLApplication::exit()
{
	// terminate glfw
	glfwTerminate();
}

// whenever the mouse is moved this callback is run
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	// get OpenGLApplication pointer from inside the window
	OpenGLApplication* app = static_cast<OpenGLApplication*>(glfwGetWindowUserPointer(window));

	// this is the first mouse movement
	if (app->m_lastMousePos == glm::vec2(-1, -1))
	{
		app->m_lastMousePos = glm::vec2(xpos, ypos);
	}

	float xoffset = (float)xpos - app->m_lastMousePos.x;
	// y position goes from bottom to top
	float yoffset = app->m_lastMousePos.y - (float)ypos;

	// store the last mouse position
	app->m_lastMousePos = glm::vec2(xpos, ypos);

	// send mouse movement to the camera
	app->m_camera.processMouseMovement(xoffset, yoffset);
}

// whenever the mouse wheel is scrolled this callback is run
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	// get OpenGLApplication pointer from inside the window
	OpenGLApplication* app = static_cast<OpenGLApplication*>(glfwGetWindowUserPointer(window));

	// send scroll to the camera
	// app->m_camera.ProcessMouseScroll(yoffset);
}

// whenever the window is resized this callback is run
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// resize viewport to match the new size
	glViewport(0, 0, width, height);
}