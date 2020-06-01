#pragma once
#define GLFW_INCLUDE_GLU
#include <GLFW\glfw3.h>

class Obstacle {
public:
	Obstacle(float r, float c1, float c2, float c3, float p1, float p2, float p3);
	void draw();
	void getPosition(float *f);
	float getRadius();

private:
	GLUquadric* quad;
	float radius;
	float position[3];
	float color[3];

};