#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class WindowHandler;

class Camera
{
public:
	glm::vec2 pos;

	glm::vec2 defaultXSides = glm::vec2(-50.0f, +50.0f);
	glm::vec2 defaultYSides = glm::vec2(-50.0f * (9.0f / 16.0f), +50.0f * (9.0f / 16.0f));

	glm::vec2 xSides;
	glm::vec2 ySides;

	float baseX = 100.0f;

	glm::vec2 zoomLimits = glm::vec2(1.0f, 5.0f);

	glm::mat4 ortho;

	GLFWwindow* window;
	float zoom = 1.0f;
	float zoomInc = 0.05;
	float dragSmth = 0.05f;
	float neededZoom = 0;
	glm::vec2 zoomPoint;
	glm::vec2 lastPos = glm::vec2(-1, -1);
	glm::vec2 dragTo = glm::vec2(0, 0);

	WindowHandler* windowHandler;

	Camera(glm::vec2 pos, WindowHandler* wh);
	void update();
	glm::vec2 getCameraCoords(glm::vec2 p);
	glm::vec2 getMouseCoords();
	void updateOrtho();
	void changeZoom(float inc);
	float limitZoom(float inZoom);
	void dragFunc(int width, int height);
};

