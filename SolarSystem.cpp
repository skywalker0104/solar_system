#include <GL/glew.h> 
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "SolarSystem.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "Camera.h"
#include "Sphere.h"
#include "Texture.h"
#include "BufferObjects.h"

#include <iostream>
#include <sstream>

using namespace std;

Camera camera;
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

enum Planets {
	Mercury, Venus, Earth,
	Moon, Mars, Jupiter,
	Saturn, Uranus, Neptune,
	Space
};

Planets PlanetViewe = Space;

bool keys[1024];

ShaderProgram planetsSheProg;
ShaderProgram sunSheProg;


GLfloat deltaTime = 0.0f;
GLfloat previousFrame = 0.0f;
//Khoi tao cac hanh tinh
Sphere sun(2);
Sphere mercury(0.06);
Sphere venus(0.16);
Sphere earth(0.18);
Sphere moon(0.08);
Sphere mars(0.1);
Sphere jupiter(0.6);
Sphere saturn(0.45);
Sphere uranus(0.225);
Sphere neptune(0.225);
Sphere skyBox(100);

//Tai chat lieu vao
Texture sunTexture("..\\textures\\sun.jpg");
Texture mercuryTexture("..\\textures\\mercury.jpg");
Texture venusTexture("..\\textures\\venus.jpg");
Texture earthTexture("..\\textures\\earth_daymap.jpg");
Texture moonTexture("..\\textures\\moon.jpg");
Texture marsTexture("..\\textures\\mars.jpg");
Texture jupiterTexture("..\\textures\\jupiter.jpg");
Texture saturnTexture("..\\textures\\saturn.jpg");
Texture uranusTexture("..\\textures\\uranus.jpg");
Texture neptuneTexture("..\\textures\\neptune.jpg");
Texture skyBoxTexture("..\\textures\\sky_box.jpg");

//Tao cua so
bool SolarSystem::createWindow(int width, int height, std::string title, bool showfullScreen)
{
	if (!glfwInit())
	{
		std::cerr << "ERROR::GLFW initialisation failed!" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	this->width = width;
	this->height = height;
	this->title = title;

	fullScreen = showfullScreen;
	if (showfullScreen)
	{
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* vidMode = glfwGetVideoMode(monitor);
		if (vidMode != nullptr)
		{
			window = glfwCreateWindow(vidMode->width, vidMode->height, title.c_str(), monitor, nullptr);
		}
	}
	else
	{
		this->window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	}



	if (window == nullptr)
	{
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(this->window);

	glfwSetKeyCallback(this->window, this->keyFunction);
	glfwSetScrollCallback(window, this->scrollFunction);
	glfwSetCursorPosCallback(window, this->mouseFunction);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		return false;
	}

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwGetFramebufferSize(window, &this->width, &this->height);
	glViewport(0, 0, this->width, this->height);
	return true;
}

//Ham nhan tin hieu cua ban phim
void SolarSystem::keyFunction(GLFWwindow* window, int key, int scancode, int action, int mode) {
	cout << key << endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

//Dieu khien camera
void SolarSystem::cameraMovement() {
	if (keys[GLFW_KEY_W])
		camera.processKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.processKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.processKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.processKeyboard(RIGHT, deltaTime);

	//if (keys[GLFW_KEY_0])
	//	PlanetViewe = Planets::Space;
}

void SolarSystem::onWindowSizeChanged() {
	int width, height;
	glfwGetWindowSize(this->window, &width, &height);
	projectionMatrix = glm::perspective(glm::radians(camera.getZoom()), float(width) / float(height), 0.5f, 1000.0f);
	glViewport(0, 0, width, height);
}

//Ket noi voi ShaderProgram de xu ly phan anh sang, chat lieu
void SolarSystem::initializeScene() {
	glEnable(GL_DEPTH_TEST);

	Shader vShader, fShader;
	vShader.loadShaderFromFile("..\\Shaders\\planet.vert", GL_VERTEX_SHADER);
	fShader.loadShaderFromFile("..\\Shaders\\planet.frag", GL_FRAGMENT_SHADER);

	if (!vShader.isShaderLoaded() || !fShader.isShaderLoaded())
	{
		return;
	}

	planetsSheProg.createProgram();
	planetsSheProg.addShaderToProgram(vShader);
	planetsSheProg.addShaderToProgram(fShader);
	vShader.deleteShader();
	fShader.deleteShader();

	if (!planetsSheProg.linkProgram())
	{
		return;
	}

	Shader vSunShader, fSunShader;
	vSunShader.loadShaderFromFile("..\\Shaders\\sun.vert", GL_VERTEX_SHADER);
	fSunShader.loadShaderFromFile("..\\Shaders\\sun.frag", GL_FRAGMENT_SHADER);

	if (!vSunShader.isShaderLoaded() || !fSunShader.isShaderLoaded())
	{
		std::cout << "==========" << std::endl;

		return;
	}

	sunSheProg.createProgram();
	sunSheProg.addShaderToProgram(vSunShader);
	sunSheProg.addShaderToProgram(fSunShader);
	vSunShader.deleteShader();
	fSunShader.deleteShader();

	if (!sunSheProg.linkProgram())
	{
		return;
	}


	// Create Buffers
	sun.generateBuffers();
	mercury.generateBuffers();
	venus.generateBuffers();
	earth.generateBuffers();
	moon.generateBuffers();
	mars.generateBuffers();
	jupiter.generateBuffers();
	saturn.generateBuffers();
	uranus.generateBuffers();
	neptune.generateBuffers();
	skyBox.generateBuffers();

	// Load textures
	sunTexture.fastLoad();
	mercuryTexture.fastLoad();
	venusTexture.fastLoad();
	earthTexture.fastLoad();
	moonTexture.fastLoad();
	marsTexture.fastLoad();
	jupiterTexture.fastLoad();
	saturnTexture.fastLoad();
	uranusTexture.fastLoad();
	neptuneTexture.fastLoad();
	skyBoxTexture.fastLoad();
}

//Ham chinh, tao ra cac hanh tinh va sap xep theo vi tri trong he mat troi
void SolarSystem::renderScene() {
	glfwPollEvents();
	cameraMovement();
	onWindowSizeChanged();

	// Clear the buffer
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	sunSheProg.useProgram();
	GLint PVM_p = glGetUniformLocation(sunSheProg.getShaderProgramID(), "PVM");

	// SKY BOX
	glBindVertexArray(skyBox.getVAO());
	glm::mat4 PVM = getProjectionMatrix();
	glUniformMatrix4fv(PVM_p, 1, GL_FALSE, glm::value_ptr(PVM));
	skyBoxTexture.useTexture();
	skyBox.draw();
	glBindVertexArray(0);

	// SUN
	glBindVertexArray(sun.getVAO());
	sun.addRotation();
	PVM = getProjectionMatrix() * camera.getViewMatrix() * sun.getModelMatrix();
	glUniformMatrix4fv(PVM_p, 1, GL_FALSE, glm::value_ptr(PVM));
	sunTexture.useTexture();
	sun.draw();
	glBindVertexArray(0);

	planetsSheProg.useProgram();
	GLint PV_p = glGetUniformLocation(planetsSheProg.getShaderProgramID(), "PV");
	GLint model_p = glGetUniformLocation(planetsSheProg.getShaderProgramID(), "model");

	// MERCURY
	glBindVertexArray(mercury.getVAO());
	mercury.addRotation(2.5f, 0.8, 25);
	glm::mat4 PV = getProjectionMatrix() * camera.getViewMatrix();
	glUniformMatrix4fv(PV_p, 1, GL_FALSE, glm::value_ptr(PV));
	glUniformMatrix4fv(model_p, 1, GL_FALSE, glm::value_ptr(mercury.getModelMatrix()));
	mercuryTexture.useTexture();
	mercury.draw();
	glBindVertexArray(0);

	// VENUS
	glBindVertexArray(venus.getVAO());
	venus.addRotation(3.5f, 0.9, 28);
	PV = getProjectionMatrix() * camera.getViewMatrix();
	glUniformMatrix4fv(PV_p, 1, GL_FALSE, glm::value_ptr(PV));
	glUniformMatrix4fv(model_p, 1, GL_FALSE, glm::value_ptr(venus.getModelMatrix()));
	venusTexture.useTexture();
	venus.draw();
	glBindVertexArray(0);

	// EARTH
	glBindVertexArray(earth.getVAO());
	earth.addRotation(4.5f, 0.8, 30);
	PV = getProjectionMatrix() * camera.getViewMatrix();
	glUniformMatrix4fv(PV_p, 1, GL_FALSE, glm::value_ptr(PV));
	glUniformMatrix4fv(model_p, 1, GL_FALSE, glm::value_ptr(earth.getModelMatrix()));
	earthTexture.useTexture();
	earth.draw();
	glBindVertexArray(0);

	// MOON
	glBindVertexArray(moon.getVAO());
	moon.addRotation(0.5f, 4, 30, earth.getOrigin());
	PV = getProjectionMatrix() * camera.getViewMatrix();
	glUniformMatrix4fv(PV_p, 1, GL_FALSE, glm::value_ptr(PV));
	glUniformMatrix4fv(model_p, 1, GL_FALSE, glm::value_ptr(moon.getModelMatrix()));
	moonTexture.useTexture();
	moon.draw();
	glBindVertexArray(0);

	// MARS
	glBindVertexArray(mars.getVAO());
	mars.addRotation(5.5f, 0.7, 34);
	PV = getProjectionMatrix() * camera.getViewMatrix();
	glUniformMatrix4fv(PV_p, 1, GL_FALSE, glm::value_ptr(PV));
	glUniformMatrix4fv(model_p, 1, GL_FALSE, glm::value_ptr(mars.getModelMatrix()));
	marsTexture.useTexture();
	mars.draw();
	glBindVertexArray(0);

	// JUPITER
	glBindVertexArray(jupiter.getVAO());
	jupiter.addRotation(7.0f, 0.6, 40);
	PV = getProjectionMatrix() * camera.getViewMatrix();
	glUniformMatrix4fv(PV_p, 1, GL_FALSE, glm::value_ptr(PV));
	glUniformMatrix4fv(model_p, 1, GL_FALSE, glm::value_ptr(jupiter.getModelMatrix()));
	jupiterTexture.useTexture();
	jupiter.draw();
	glBindVertexArray(0);

	// SATURN
	glBindVertexArray(saturn.getVAO());
	saturn.addRotation(8.5f, 0.5, 35);
	PV = getProjectionMatrix() * camera.getViewMatrix();
	glUniformMatrix4fv(PV_p, 1, GL_FALSE, glm::value_ptr(PV));
	glUniformMatrix4fv(model_p, 1, GL_FALSE, glm::value_ptr(saturn.getModelMatrix()));
	saturnTexture.useTexture();
	saturn.draw();
	glBindVertexArray(0);

	// URANUS
	glBindVertexArray(uranus.getVAO());
	uranus.addRotation(9.5f, 0.7, 40);
	PV = getProjectionMatrix() * camera.getViewMatrix();
	glUniformMatrix4fv(PV_p, 1, GL_FALSE, glm::value_ptr(PV));
	glUniformMatrix4fv(model_p, 1, GL_FALSE, glm::value_ptr(uranus.getModelMatrix()));
	uranusTexture.useTexture();
	uranus.draw();
	glBindVertexArray(0);

	// NEPTUNE
	glBindVertexArray(neptune.getVAO());
	neptune.addRotation(10.0f, 0.85f, 40);
	PV = getProjectionMatrix() * camera.getViewMatrix();
	glUniformMatrix4fv(PV_p, 1, GL_FALSE, glm::value_ptr(PV));
	glUniformMatrix4fv(model_p, 1, GL_FALSE, glm::value_ptr(neptune.getModelMatrix()));
	neptuneTexture.useTexture();
	neptune.draw();
	glBindVertexArray(0);


	// Swap the screen buffers
	glfwSwapBuffers(this->window);
}

//Ham chay chuong trinh sau khi da setup xong moi thu
void SolarSystem::runApp() {
	initializeScene();
	while (!glfwWindowShouldClose(this->window))
	{
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - previousFrame;
		previousFrame = currentFrame;
		renderScene();
	}
}

SolarSystem::~SolarSystem() {
	glfwTerminate();
}

//ham lay tin hieu cua con tro chuot
void SolarSystem::mouseFunction(GLFWwindow* window, double x, double y) {
	if (firstMouse)
	{
		lastX = x;
		lastY = y;
		firstMouse = false;
	}

	GLfloat xoffset = x - lastX;
	GLfloat yoffset = lastY - y;  // Reversed since y-coordinates go from bottom to left

	lastX = x;
	lastY = y;

	camera.processMouseMovement(xoffset, yoffset);
}

//Lay tin hieu cua con lan
void SolarSystem::scrollFunction(GLFWwindow* window, double x, double y) {
	camera.processMouseScroll(y);
}