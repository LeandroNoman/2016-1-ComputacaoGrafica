#define _USE_MATH_DEFINES
#include "Boid.h"
#include <stdio.h>
#include <math.h>

Boid::Boid(Boss &boss, std::vector<Boid> &boids, int boidID, Cone &cone, std::vector<Obstacle> &obs) {
	quad = gluNewQuadric();
	speed[0] = 0;
	speed[1] = 0;
	speed[2] = 0;
	force[0] = 0;
	force[1] = 0;
	force[2] = 0;
	update(boss, boids, boidID, cone, obs);
}

Boid::Boid(float p0, float p1, float p2,
	Boss &boss, std::vector<Boid> &boids, int boidID, Cone &cone, std::vector<Obstacle> &obs) {
	quad = gluNewQuadric();
	speed[0] = 0;
	speed[1] = 0;
	speed[2] = 0;
	force[0] = 0;
	force[1] = 0;
	force[2] = 0;
	position[0] = p0;
	position[1] = p1;
	position[2] = p2;
	update(boss, boids, boidID, cone, obs);
}

void Boid::getPosition(float *f) {
	f[0] = position[0];
	f[1] = position[1];
	f[2] = position[2];
}

float normalizeVect(float v[3]) {
	float length;

	length = sqrtf((v[0] * v[0]) + (v[1] * v[1]) + (v[2] * v[2]));

	v[0] /= length;
	v[1] /= length;
	v[2] /= length;

	return length;
}

void Boid::updateSpeedMagnitude(float dist) {
	speedMagnitude = (dist - 1) / 80;
}

void Boid::updateSpeed() {
	speed[0] += force[0];
	speed[1] += force[1];
	speed[2] += force[2];
}

void Boid::updatePosition() {
	position[0] += speed[0];
	position[1] += speed[1];
	position[2] += speed[2];
}

void Boid::update(Boss &boss, std::vector<Boid> &boids, int boidID, Cone &cone, std::vector<Obstacle> &obs) {
	int id;
	float distance;
	float vec3d[3];

	//Create attraction force to boss
	boss.getPosition(vec3d);
	vec3d[0] -= position[0];
	vec3d[1] -= position[1];
	vec3d[2] -= position[2];
	distance = normalizeVect(vec3d);
	//Updates the boids speed based on the distance to the objective
	updateSpeedMagnitude(distance);

	speed[0] = vec3d[0] * speedMagnitude;
	speed[1] = vec3d[1] * speedMagnitude;
	speed[2] = vec3d[2] * speedMagnitude;

	id = 0;
	//Verify distance to each other boid
	for (std::vector<Boid>::iterator it = boids.begin(); it != boids.end(); it++) {
		if (id == boidID) {
			continue;
		}
		(*it).getPosition(vec3d);
		vec3d[0] -= position[0];
		vec3d[1] -= position[1];
		vec3d[2] -= position[2];
		distance = normalizeVect(vec3d);

		//Separation and Cohesion
		speed[0] -= (vec3d[0] * separationRate) / (distance * distance);
		speed[1] -= (vec3d[1] * separationRate) / (distance * distance);
		speed[2] -= (vec3d[2] * separationRate) / (distance * distance);

		id++;
	}

	//Verify the distance to the central cone
	vec3d[0] = 0.f - position[0] / cone.getBaseArea();
	vec3d[1] = 0.f;
	vec3d[2] = 0.f - position[2] / cone.getBaseArea();
	distance = normalizeVect(vec3d);

	speed[0] -= vec3d[0] / distance;
	speed[1] -= vec3d[1] / distance;
	speed[2] -= vec3d[2] / distance;

	//Verify the distance to every obstacle
	for (std::vector<Obstacle>::iterator it = obs.begin(); it != obs.end(); it++) {
		(*it).getPosition(vec3d);
		vec3d[0] -= position[0];
		vec3d[1] -= position[1];
		vec3d[2] -= position[2];
		distance = normalizeVect(vec3d);

		speed[0] -= vec3d[0] / (distance - (*it).getRadius());
		speed[1] -= vec3d[1] / (distance - (*it).getRadius());
		speed[2] -= vec3d[2] / (distance - (*it).getRadius());
	}

	updatePosition();
}

void Boid::draw(int drawMode) {
	if (drawMode == DRAWSPHERE) {
		glTranslatef(position[0], position[1], position[2]);
		glColor3f(1.0, 1.0, 1.0);
		gluSphere(quad, 1.0, 20, 20);
	}
	else if (drawMode == DRAWBIRD) {
		float uniSpeed[3] = { speed[0] / speedMagnitude, speed[1] / speedMagnitude, speed[2] / speedMagnitude };
		float vecProd[3] = { -uniSpeed[1], uniSpeed[0], 0.0f };
		if (uniSpeed[2] > 1.0f) {
			uniSpeed[2] = 1.0f;
		}
		else if (uniSpeed[2] < -1.0f) {
			uniSpeed[2] = -1.0f;
		}
		float angle = acosf(uniSpeed[2]);
		//float testAngle = acosf(vecProd[0] * uniSpeed[0] + vecProd[2] * uniSpeed[2] + vecProd[2] * uniSpeed[2]);

		//printf("Angle %.2f | Vec: %.2f %.2f %.2f | %.2f\n", angle, vecProd[0], vecProd[1], vecProd[2], testAngle);


		glTranslatef(position[0], position[1], position[2]);
		glRotatef(static_cast<float>((angle / M_PI)) * 180, vecProd[0], vecProd[1], vecProd[2]);
		glColor3f(1.0f, 0.0f, 0.0f);
		glDisable(GL_CULL_FACE);
		glBegin(GL_TRIANGLES);
		glVertex3f(0, 0, 1);
		glVertex3f(1, 1, -1);
		glVertex3f(1, -1, -1);

		glVertex3f(0, 0, 1);
		glVertex3f(1, -1, -1);
		glVertex3f(-1, -1, -1);

		glVertex3f(0, 0, 1);
		glVertex3f(-1, -1, -1);
		glVertex3f(-1, 1, -1);

		glVertex3f(0, 0, 1);
		glVertex3f(1, 1, -1);
		glVertex3f(-1, 1, -1);
		glEnd();

		glColor3f(1.0f, 1.0f, 0.0f);

		glBegin(GL_TRIANGLES);
		glVertex3f(0, 0, -4);
		glVertex3f(1, 1, -1);
		glVertex3f(1, -1, -1);

		glVertex3f(0, 0, -4);
		glVertex3f(1, -1, -1);
		glVertex3f(-1, -1, -1);

		glVertex3f(0, 0, -4);
		glVertex3f(-1, -1, -1);
		glVertex3f(-1, 1, -1);

		glVertex3f(0, 0, -4);
		glVertex3f(1, 1, -1);
		glVertex3f(-1, 1, -1);
		glEnd();

		glBegin(GL_TRIANGLES);
		glVertex3f(0, 0, -4);
		glVertex3f(1, 1, -4.7f);
		glVertex3f(1, -1, -4.7f);

		glVertex3f(0, 0, -4);
		glVertex3f(1, -1, -4.7f);
		glVertex3f(-1, -1, -4.7f);

		glVertex3f(0, 0, -4);
		glVertex3f(-1, -1, -4.7f);
		glVertex3f(-1, 1, -4.7f);

		glVertex3f(0, 0, -4);
		glVertex3f(1, 1, -4.7f);
		glVertex3f(-1, 1, -4.7f);
		glEnd();

		glBegin(GL_TRIANGLES);
		glVertex3f(-4, 0, -2.5f);
		glVertex3f(0, 0, -0.5f);
		glVertex3f(0, -0.5f, -2.5f);

		glVertex3f(-4, 0, -2.5f);
		glVertex3f(0, 0, -0.5f);
		glVertex3f(0, 0.5f, -2.5f);

		glVertex3f(-4, 0, -2.5f);
		glVertex3f(0, -0.5f, -2.5f);
		glVertex3f(0, 0, -4);

		glVertex3f(-4, 0, -2.5f);
		glVertex3f(0, 0.5f, -2.5f);
		glVertex3f(0, 0, -4);
		glEnd();

		glBegin(GL_TRIANGLES);
		glVertex3f(4, 0, -2.5f);
		glVertex3f(0, 0, -0.5f);
		glVertex3f(0, -0.5f, -2.5f);

		glVertex3f(4, 0, -2.5f);
		glVertex3f(0, 0, -0.5f);
		glVertex3f(0, 0.5f, -2.5f);

		glVertex3f(4, 0, -2.5f);
		glVertex3f(0, -0.5f, -2.5f);
		glVertex3f(0, 0, -4);

		glVertex3f(4, 0, -2.5f);
		glVertex3f(0, 0.5f, -2.5f);
		glVertex3f(0, 0, -4);
		glEnd();
		glEnable(GL_CULL_FACE);
	}
}

void Boid::printState(int boidID) {
	printf("Boid %d: Velocidade: %.2f | Posicao: %.2f %.2f %.2f\n", boidID, speedMagnitude, position[0], position[1], position[2]);
}