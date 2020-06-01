#define _USE_MATH_DEFINES
#include "Boss.h"
#include <stdio.h>
#include <math.h>

#define HALFPI 1.570796f
#define PI 3.1415926f
#define TWOPI 6.283185f
#define DEGREE 0.01745f

Boss::Boss() {
	quad = gluNewQuadric();
}

void Boss::getPosition(float *f) {
	f[0] = position[0];
	f[1] = position[1];
	f[2] = position[2];
}

void Boss::setPosition(float f0, float f1, float f2) {
	position[0] = f0;
	position[1] = f1;
	position[2] = f2;
}

void Boss::getSpeed(float *f) {
	f[0] = speedMagnitude * sinf(yAngle) * sinf(zAngle);
	f[1] = speedMagnitude * cosf(yAngle);
	f[2] = speedMagnitude * sinf(yAngle) * cosf(zAngle);
}

void Boss::setSpeed(float s) {
	speedMagnitude = s;
}

void Boss::setYAngle(float z) {
	yAngle = z;
}

void Boss::setZAngle(float x) {
	zAngle = x;
}

void Boss::update(BoundingBox &bb) {
	float scale = bb.getScaleValue();
	float distance;

	//Checks proximity with the 5 planes
	//Lado 1 (z = -1)
	distance = fabsf(position[2] - (scale * -1.f));
	if (distance < 5.0f) {
		if (zAngle <= PI) {
			zAngle -= zAngle * ((5.0f - distance) / 5.0f);
		}
		else {
			zAngle += (TWOPI - zAngle) * ((5.0f - distance) / 5.0f);
		}
	}
	//Lado 2 (x = 1)
	distance = fabsf(position[0] - scale);
	if (distance < 5.0f) {
		if (zAngle <= HALFPI) {
			zAngle -= (zAngle - (PI + HALFPI)) * ((5.0f - distance) / 5.0f);
		}
		else if (zAngle >= PI + HALFPI) {
			zAngle += zAngle * ((5.0f - distance) / 5.0f);
		}
		else {
			zAngle += ((PI + HALFPI) - zAngle) * ((5.0f - distance) / 5.0f);
		}
	}
	//Lado 3 (z = 1)
	distance = fabsf(position[2] - scale);
	if (distance < 5.0f) {
		if (zAngle <= PI) {
			zAngle += (PI - zAngle) * ((5.0f - distance) / 5.0f);
		}
		else {
			zAngle -= (zAngle - PI) * ((5.0f - distance) / 5.0f);
		}
	}
	//Lado 4 (x = -1)
	distance = fabsf(position[0] - (scale * -1.f));
	if (distance < 5.0f) {
		if (zAngle <= PI + HALFPI && zAngle >= HALFPI) {
			zAngle -= (zAngle - HALFPI) * ((5.0f - distance) / 5.0f);
		}
		else if (zAngle <= HALFPI) {
			zAngle += (HALFPI - zAngle) * ((5.0f - distance) / 5.0f);
		}
		else {
			zAngle -= (TWOPI - zAngle + HALFPI) * ((5.0f - distance) / 5.0f);
		}
	}

	if (zAngle < 0.0f) {
		zAngle = TWOPI - zAngle;
	}
	else if (zAngle > TWOPI) {
		zAngle -= TWOPI;
	}
	//Fundo (y = 0)
	distance = position[1];
	if (distance < 5.0f) {
		if (yAngle <= PI) {
			yAngle -= yAngle * ((5.0f - distance) / 5.0f);
		}
		else {
			yAngle += (TWOPI - yAngle) * ((5.0f - distance) / 5.0f);
		}
	}

	position[0] += speedMagnitude * sinf(yAngle) * sinf(zAngle);
	position[1] += speedMagnitude * cosf(yAngle);
	position[2] += speedMagnitude * sinf(yAngle) * cosf(zAngle);
}

void Boss::rotateSpeedVectorManual(int keys) {
	if(keys == 0) {
		return;
	}
	//UP
	else if ((keys & 0x1) > 0) {
		if (yAngle <= PI) {
			yAngle -= DEGREE;
		}
		else {
			yAngle += DEGREE;
		}
	}
	//RIGHT
	else if ((keys & 0x2) > 0) {
		zAngle -= DEGREE;
	}
	//DOWN
	else if ((keys & 0x4) > 0) {
		if (yAngle >= PI) {
			yAngle -= DEGREE;
		}
		else {
			yAngle += DEGREE;
		}
	}
	//LEFT
	else if ((keys & 0x8) > 0) {
		zAngle += DEGREE;
	}

	if (yAngle < 0) {
		yAngle = TWOPI + yAngle;
	}
	else if (yAngle > TWOPI) {
		yAngle -= TWOPI;
	}
	if (zAngle < 0) {
		zAngle = TWOPI + zAngle;
	}
	else if (zAngle > TWOPI) {
		zAngle -= TWOPI;
	}
}

void Boss::draw(int drawMode) {
	if (drawMode == DRAWSPHERE) {
		glTranslatef(position[0], position[1], position[2]);
		glColor3f(1.0, 1.0, 1.0);
		gluSphere(quad, 1.0, 20, 20);
	}
	else if (drawMode == DRAWBIRD) {
		float speed[3];
		speed[0] = speedMagnitude * sinf(yAngle) * sinf(zAngle);
		speed[1] = speedMagnitude * cosf(yAngle);
		speed[2] = speedMagnitude * sinf(yAngle) * cosf(zAngle);

		float uniSpeed[3] = { speed[0] / speedMagnitude, speed[1] / speedMagnitude, speed[2] / speedMagnitude };
		float vecProd[3] = { -uniSpeed[1], uniSpeed[0], 0.0f };
		float angle = acosf(uniSpeed[2]);
		//float testAngle = acosf(vecProd[0] * uniSpeed[0] + vecProd[1] * uniSpeed[1] + vecProd[2] * uniSpeed[2]);

		//printf("Angle %.2f | Vec: %.2f %.2f %.2f | %.2f\n", angle, vecProd[0], vecProd[1], vecProd[2], (testAngle / M_PI) * 180);


		glTranslatef(position[0], position[1], position[2]);
		glRotatef(static_cast <float>((angle / M_PI)) * 180, vecProd[0], vecProd[1], vecProd[2]);
		glColor3f(0.0f, 0.0f, 0.0f);
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
		glVertex3f(0, -0.5, -2.5f);

		glVertex3f(-4, 0, -2.5f);
		glVertex3f(0, 0, -0.5f);
		glVertex3f(0, 0.5, -2.5f);

		glVertex3f(-4, 0, -2.5f);
		glVertex3f(0, -0.5f, -2.5f);
		glVertex3f(0, 0, -4);

		glVertex3f(-4, 0, -2.5f);
		glVertex3f(0, 0.5f, -2.5f);
		glVertex3f(0, 0, -4);
		glEnd();

		glBegin(GL_TRIANGLES);
		glVertex3f(4, 0, -2.5);
		glVertex3f(0, 0, -0.5);
		glVertex3f(0, -0.5, -2.5);

		glVertex3f(4, 0, -2.5);
		glVertex3f(0, 0, -0.5);
		glVertex3f(0, 0.5, -2.5);

		glVertex3f(4, 0, -2.5);
		glVertex3f(0, -0.5, -2.5);
		glVertex3f(0, 0, -4);

		glVertex3f(4, 0, -2.5);
		glVertex3f(0, 0.5, -2.5);
		glVertex3f(0, 0, -4);
		glEnd();
		glEnable(GL_CULL_FACE);
	}
}

void Boss::printState(int boidID) {
	printf("Boid %d: Velocidade: %.2f | Angulo Y: %.2f | Angulo Z: %.2f\n        Posicao: %.2f %.2f %.2f\n", boidID, speedMagnitude, yAngle, zAngle, position[0], position[1], position[2]);
}