#pragma once
#define GLFW_INCLUDE_GLU
#include <GLFW\glfw3.h>

class Cone {
public:
	Cone();
	void draw();
	float getHeigh();
	float getBaseArea();

private:
	GLUquadric* quad;

};