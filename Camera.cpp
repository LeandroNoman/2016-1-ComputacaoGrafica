#include "Camera.h"
#define GLFW_INCLUDE_GLU
#include <GLFW\glfw3.h>

void Camera::goToModelView() {
	gluLookAt(eyePosition[0], eyePosition[1], eyePosition[2],
		lookAt[0], lookAt[1], lookAt[2],
		upVector[0], upVector[1], upVector[2]);
}

void Camera::positionEye(double x, double y, double z) {
	eyePosition[0] = x;
	eyePosition[1] = y;
	eyePosition[2] = z;
}

void Camera::aimAt(double x, double y, double z) {
	lookAt[0] = x;
	lookAt[1] = y;
	lookAt[2] = z;
}