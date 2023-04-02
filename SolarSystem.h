#ifndef SolarSystem_H
#define SolarSystem_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>

class SolarSystem {
	GLFWwindow* window = nullptr;
	std::string title;

	int width = 800, height = 600;
	bool fullScreen = true;

	glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), float(width) / float(height), 0.5f, 1000.0f);

public:
	~SolarSystem();

	bool createWindow(int width, int height, std::string title, bool showfullScreen);
	void runApp();
	GLFWwindow* getWindow() const { return window; };
	glm::mat4 getProjectionMatrix() const { return projectionMatrix; };

private:
	void initializeScene();
	void cameraMovement();
	void renderScene();
	static void keyFunction(GLFWwindow* window, int key, int scancode, int action, int mode);
	static void mouseFunction(GLFWwindow* window, double x, double y);
	static void scrollFunction(GLFWwindow* window, double x, double y);

	void onWindowSizeChanged();
};

#endif // !SolarSystem_H
