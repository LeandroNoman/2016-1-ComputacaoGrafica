#pragma once
#include <vector>
#define GLFW_INCLUDE_GLU
#include <GLFW\glfw3.h>
#include "Boss.h"
#include "Cone.h"
#include "Obstacle.h"

#define DRAWSPHERE 0
#define DRAWBIRD 1

class Boid {
public:
	Boid(Boss &boss, std::vector<Boid> &boids, int boidID, Cone &cone, std::vector<Obstacle> &obs);
	Boid(float position0, float position1, float position2,
		Boss &boss, std::vector<Boid> &boids, int boidID, Cone &cone, std::vector<Obstacle> &obs);
	void getPosition(float *f);
	void update(Boss &boss, std::vector<Boid> &boids, int boidID, Cone &cone, std::vector<Obstacle> &obs);
	void draw(int drawMode);
	void printState(int boidID);

private:
	GLUquadric* quad;
	float separationRate = 1.0f;
	float speedMagnitude = 0.0f;
	float position[3], speed[3], force[3];

	void updateSpeedMagnitude(float distance);
	void updateSpeed();
	void updatePosition();

};