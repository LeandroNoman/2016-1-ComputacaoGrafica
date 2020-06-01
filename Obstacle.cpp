#include "Obstacle.h"

Obstacle::Obstacle(float r, float c1, float c2, float c3, float p1, float p2, float p3) {
	quad = gluNewQuadric();
	radius = r;
	color[0] = c1;
	color[1] = c2;
	color[2] = c3;
	position[0] = p1;
	position[1] = p2;
	position[2] = p3;
}

void Obstacle::draw() {
	glTranslatef(position[0], position[1], position[2]);
	glColor3fv(color);
	gluSphere(quad, radius, 20, 20);
}

void Obstacle::getPosition(float *f) {
	f[0] = position[0];
	f[1] = position[1];
	f[2] = position[2];
}

float Obstacle::getRadius() {
	return radius;
}