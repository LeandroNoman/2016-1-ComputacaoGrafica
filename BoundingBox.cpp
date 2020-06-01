#include "BundingBox.h"
#include <GLFW\glfw3.h>

float BoundingBox::getScaleValue() {
	return scale;
}

void BoundingBox::draw() {
	glScalef(scale, scale, scale);

	glBegin(GL_QUADS);

	glColor3f(0.0f, 1.0f, 0.0f);
	//Lado 1 (z = -1)
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 0.0f, -1.0f);
	glVertex3f(1.0f, 0.0f, -1.0f);

	glColor3f(1.0f, 0.0f, 0.0f);
	//Lado 2 (x = 1)
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 0.0f, -1.0f);
	glVertex3f(1.0f, 0.0f, 1.0f);

	glColor3f(0.0f, 0.0f, 1.0f);
	//Lado 3 (z = 1)
	glVertex3f(1.0f, 0.0f, 1.0f);
	glVertex3f(-1.0f, 0.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	//Lado 4 (x = -1)
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 0.0f, 1.0f);
	glVertex3f(-1.0f, 0.0f, -1.0f);

	glColor3f(1.0f, 0.0f, 1.0f);
	//Fundo (y = 0)
	glVertex3f(1.0f, 0.0f, -1.0f);
	glVertex3f(-1.0f, 0.0f, -1.0f);
	glVertex3f(-1.0f, 0.0f, 1.0f);
	glVertex3f(1.0f, 0.0f, 1.0f);

	glEnd();
}