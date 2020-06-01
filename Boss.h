#pragma once
#define GLFW_INCLUDE_GLU
#include <GLFW\glfw3.h>
#include "BundingBox.h"

#define DRAWSPHERE 0
#define DRAWBIRD 1

class Boss {
public:
	Boss();
	void getPosition(float *f);
	void setPosition(float f0, float f1, float f2);
	void getSpeed(float *f);
	void setSpeed(float s);
	void setYAngle(float y);
	void setZAngle(float z);
	void update(BoundingBox &bb);
	void rotateSpeedVectorManual(int keys);
	void draw(int drawMode);
	void printState(int boidID);

private:
	GLUquadric* quad;
	float position[3];
	float yAngle, zAngle, speedMagnitude;

};