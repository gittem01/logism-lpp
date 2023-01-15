#include <Camera.h>
#include <WindowHandler.h>

Camera::Camera(glm::vec2 pos, WindowHandler* wh)
{
    this->pos = pos;
    this->window = wh->window;
    this->windowHandler = wh;

    xSides = this->defaultXSides / (this->zoom * this->zoom) + this->pos.x;
    ySides = this->defaultYSides / (this->zoom * this->zoom) + this->pos.y;

    updateOrtho();
}

void Camera::updateOrtho()
{
	xSides = defaultXSides / (zoom * zoom) + pos.x;
	ySides = defaultYSides / (zoom * zoom) + pos.y;

	ortho = glm::ortho(xSides.x, xSides.y, ySides.x, ySides.y, -100.0f, +100.0f);
}

void Camera::changeZoom(float inc)
{
	glm::vec2 mp = getCameraCoords(zoomPoint);

	float zoomAfter = limitZoom(zoom + inc);

	glm::vec2 xSidesAfter = defaultXSides / (zoomAfter * zoomAfter) + pos.x;
	glm::vec2 ySidesAfter = defaultYSides / (zoomAfter * zoomAfter) + pos.y;

	float xPerctBefore = (mp.x - pos.x) / (xSides.y - xSides.x);
	float xPerctAfter = (mp.x - pos.x) / (xSidesAfter.y - xSidesAfter.x);

	float yPerctBefore = (mp.y - pos.y) / (ySides.y - ySides.x);
	float yPerctAfter = (mp.y - pos.y) / (ySidesAfter.y - ySidesAfter.x);

	pos.x += (xPerctAfter - xPerctBefore) * (xSidesAfter.y - xSidesAfter.x);
	pos.y += (yPerctAfter - yPerctBefore) * (ySidesAfter.y - ySidesAfter.x);
	zoom += inc;
	zoom = limitZoom(zoom);
}

float Camera::limitZoom(float inZoom)
{
	if (inZoom < zoomLimits.x)
	{
		inZoom = zoomLimits.x;
	}
	else if (inZoom > zoomLimits.y)
	{
		inZoom = zoomLimits.y;
	}

	return inZoom;
}

void Camera::update()
{
	if (windowHandler->windowSizes.x == 0 || windowHandler->windowSizes.y == 0)
	{
		return;
	}

	dragFunc(windowHandler->windowSizes.x, windowHandler->windowSizes.y);
	if (windowHandler->mouseData[5] != 0 &&
		!windowHandler->keyData[GLFW_KEY_LEFT_CONTROL] &&
		!windowHandler->keyData[GLFW_KEY_RIGHT_CONTROL] &&
		!windowHandler->keyData[GLFW_KEY_LEFT_SHIFT])
	{
		neededZoom += zoom * windowHandler->mouseData[5] / 10.0f;
		zoomPoint.x = windowHandler->mouseData[0]; zoomPoint.y = windowHandler->mouseData[1];
	}

	if (neededZoom != 0)
	{
		changeZoom(zoomInc * neededZoom);
		neededZoom -= zoomInc * neededZoom;
	}

	updateOrtho();
}

void Camera::dragFunc(int width, int height) 
{
	glm::vec2 diffVec = glm::vec2(dragTo.x - lastPos.x, dragTo.y - lastPos.y);
	glm::vec2 sideDiffs = glm::vec2(defaultXSides.y - defaultXSides.x, defaultYSides.y - defaultYSides.x);

	float lng = glm::length(diffVec);

	if (abs(lng) > 0) 
	{
		pos.x -= diffVec.x * (sideDiffs.x / width) * dragSmth / (zoom * zoom);
		pos.y -= diffVec.y * (sideDiffs.y / height) * dragSmth / (zoom * zoom);

		lastPos.x += diffVec.x * dragSmth;
		lastPos.y += diffVec.y * dragSmth;
	}

	if (windowHandler->keyData[GLFW_KEY_LEFT_SHIFT]) 
	{
		pos.x -= windowHandler->trackpadData[0] * (sideDiffs.x / width) * dragSmth / (zoom * zoom) * 5.0f;
		pos.y -= windowHandler->trackpadData[1] * (sideDiffs.y / height) * dragSmth / (zoom * zoom) * 5.0f;
	}
	else 
	{
		if (windowHandler->mouseData[4] == 2) 
		{
			lastPos.x = windowHandler->mouseData[0];
			lastPos.y = windowHandler->mouseData[1];

			dragTo.x = windowHandler->mouseData[0];
			dragTo.y = windowHandler->mouseData[1];
		}
		else if (windowHandler->mouseData[4] == 1) 
		{
			dragTo.x = windowHandler->mouseData[0];
			dragTo.y = windowHandler->mouseData[1];
		}
	}
}

glm::vec2 Camera::getMouseCoords()
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	float xPerct = windowHandler->mouseData[0] / (float)width;
	float yPerct = 1.0f - windowHandler->mouseData[1] / (float)height;

	glm::vec2 xSides = defaultXSides / (zoom * zoom) + pos.x;
	float xDiff = xSides.y - xSides.x;

	glm::vec2 ySides = defaultYSides / (zoom * zoom) + pos.y;
	float yDiff = ySides.y - ySides.x;

	float xPos = xSides.x + xPerct * xDiff;
	float yPos = ySides.y - yPerct * yDiff;

	return glm::vec2(xPos, yPos);
}

glm::vec2 Camera::getCameraCoords(glm::vec2 p)
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	float xPerct = p.x / width;
	float yPerct = p.y / height;

	glm::vec2 xSides = defaultXSides / (zoom * zoom) + pos.x;
	float xDiff = xSides.y - xSides.x;

	glm::vec2 ySides = defaultYSides / (zoom * zoom) + pos.y;
	float yDiff = ySides.y - ySides.x;

	float xPos = xSides.x + xPerct * xDiff;
	float yPos = ySides.x + yPerct * yDiff;

	return glm::vec2(xPos, yPos);
}