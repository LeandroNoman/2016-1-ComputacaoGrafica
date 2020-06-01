#include "Cone.h"
#include <GLFW\glfw3.h>

Cone::Cone() {
	quad = gluNewQuadric();
}

void Cone::draw() {
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	glColor3f(1.0, 1.0, 1.0);
	gluCylinder(quad, 10.0, 0.0, 90.0, 20, 20);
}

float Cone::getHeigh() {
	return 90.0f;
}

float Cone::getBaseArea() {
	return 10.0f;
}