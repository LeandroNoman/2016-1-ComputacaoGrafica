#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <math.h>
#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>
#include "BundingBox.h"
#include "Cone.h"
#include "Boss.h"
#include "Boid.h"
#include "Camera.h"
#include "Obstacle.h"

#define SCREENWIDTH 854
#define SCREENHEIGHT 480

#define PI 3.1415926f

#define CONEMODE 0
#define BOSSMODE 1
#define BOIDMODE 2

#define DRAWSPHERE 0
#define DRAWBIRD 1

BoundingBox boundingBox;
Cone cone;
Boss boss;
Camera camera;
std::vector<Boid> boids;
std::vector<Obstacle> obstacles;

int windowWidth = SCREENWIDTH;
int windowHeight = SCREENHEIGHT;
bool running = true;
bool paused = true;
bool runonce = false;
int cameraMode = CONEMODE;
int bossMovement = 0;
int drawMode = DRAWSPHERE;

void positionCamera();
void createRandomBoid();
void deleteRandomBoid();

void mousebutton_callback(GLFWwindow* window, int button, int action, int mods) {
	switch (button) {
	case GLFW_MOUSE_BUTTON_LEFT:
		if (action == GLFW_PRESS) {
			cameraMode = (cameraMode + 1) % 3;
			positionCamera();
		}
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		if (action == GLFW_PRESS) {
			if (paused == true) {
				runonce = true;
			}
		}
		break;
	}
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
	windowWidth = width;
	windowHeight = height;
}

void character_callback(GLFWwindow *window, unsigned int codepoint) {
	switch (codepoint) {
	case 'q':
	case 'Q':
		running = false;
		break;
	case 'p':
	case 'P':
		if (paused == true) {
			paused = false;
		}
		else {
			paused = true;
		}
		break;
	case ' ':
		cameraMode = (cameraMode + 1) % 3;
		positionCamera();
		break;
	case 'l':
	case 'L':
		if (glIsEnabled(GL_LIGHTING) == GL_TRUE) {
			glDisable(GL_LIGHTING);
		}
		else {
			glEnable(GL_LIGHTING);
		}
		break;
	case 'f':
	case 'F':
		if (glIsEnabled(GL_FOG) == GL_TRUE) {
			glDisable(GL_FOG);
		}
		else {
			glEnable(GL_FOG);
		}
		break;
	case 'd':
	case 'D':
		drawMode = (drawMode + 1) % 2;
		break;
	case '=':
	case '+':
		createRandomBoid();
		break;
	case '_':
	case '-':
		deleteRandomBoid();
		break;
	}
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	switch (key) {
	case GLFW_KEY_UP:
		if (action == GLFW_PRESS) {
			bossMovement |= 0x1;
		}
		else if (action == GLFW_RELEASE) {
			bossMovement &= 0xFE;
		}
		break;
	case GLFW_KEY_RIGHT:
		if (action == GLFW_PRESS) {
			bossMovement |= 0x2;
		}
		else if (action == GLFW_RELEASE) {
			bossMovement &= 0xFD;
		}
		break;
	case GLFW_KEY_DOWN:
		if (action == GLFW_PRESS) {
			bossMovement |= 0x4;
		}
		else if (action == GLFW_RELEASE) {
			bossMovement &= 0xFB;
		}
		break;
	case GLFW_KEY_LEFT:
		if (action == GLFW_PRESS) {
			bossMovement |= 0x8;
		}
		else if (action == GLFW_RELEASE) {
			bossMovement &= 0xF7;
		}
		break;
	}
}


bool initGL(GLFWwindow** window) {
	if (!glfwInit()) {
		return false;
	}

	*window = glfwCreateWindow(SCREENWIDTH, SCREENHEIGHT, "Boids", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(*window);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_FOG);
	glEnable(GL_CULL_FACE);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat) SCREENWIDTH / (GLfloat) SCREENHEIGHT, 0.1f, 300.0f);
	glMatrixMode(GL_MODELVIEW);

	GLfloat lightPos[] = { .0, 50.0, .0 };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	glFogf(GL_FOG_MODE, GL_LINEAR);
	glFogf(GL_FOG_END, 300.0);

	glfwSetFramebufferSizeCallback(*window, framebuffer_size_callback);
	glfwSetCharCallback(*window, character_callback);
	glfwSetKeyCallback(*window, key_callback);
	glfwSetMouseButtonCallback(*window, mousebutton_callback);
	glfwSwapInterval(1);

	return true;
}

void clearScreen() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 0);
}

void createObstacles() {
	obstacles.push_back(Obstacle(1.0, 1.0, 0., 0., 20., 20., 20.));
	obstacles.push_back(Obstacle(2.0, 0., 1.0, 0., 0., 40., 30.));
	obstacles.push_back(Obstacle(5.0, 1.0, 0., 1.0, 50., 80., -20.));
	obstacles.push_back(Obstacle(6.0, .5, .5, .5, -60., 60., 60.));
	obstacles.push_back(Obstacle(4.0, 1.0, 1.0, 0., -40., 40., -10.));
	obstacles.push_back(Obstacle(10.0, 0., 0., 1., -40., 80., -50.));
	obstacles.push_back(Obstacle(7.0, 0., 1., 1., 55., 60., -10.));
}

void normalizeVect(float v[3]) {
	float length;

	length = sqrtf((v[0] * v[0]) + (v[1] * v[1]) + (v[2] * v[2]));

	v[0] /= length;
	v[1] /= length;
	v[2] /= length;
}

void getBoidsCenter(float *center) {
	int i;
	float vec3d[3];

	center[0] = 0.0f;
	center[1] = 0.0f;
	center[2] = 0.0f;

	i = 0;
	for (std::vector<Boid>::iterator it = boids.begin(); it != boids.end(); it++) {
		(*it).getPosition(vec3d);
		center[0] += vec3d[0];
		center[1] += vec3d[1];
		center[2] += vec3d[2];
		i++;
	}

	center[0] /= i;
	center[1] /= i;
	center[2] /= i;
}

void positionCamera() {
	float vectPos[3], vectSpd[3], vectCenter[3];

	switch (cameraMode)	{
	case CONEMODE:
		boss.getPosition(vectPos);

		camera.positionEye(0.0, 100.0, 0.0);
		camera.aimAt(vectPos[0], vectPos[1], vectPos[2]);
		break;
	case BOSSMODE:
		boss.getPosition(vectPos);
		boss.getSpeed(vectSpd);
		normalizeVect(vectSpd);
		vectSpd[0] = vectPos[0] - (16 * vectSpd[0]);
		vectSpd[1] = vectPos[1] - (16 * vectSpd[1]);
		vectSpd[2] = vectPos[2] - (16 * vectSpd[2]);

		camera.positionEye(vectSpd[0], vectSpd[1], vectSpd[2]);
		camera.aimAt(vectPos[0], vectPos[1], vectPos[2]);
		break;
	case BOIDMODE:
		getBoidsCenter(vectCenter);
		boss.getPosition(vectPos);
		vectSpd[0] = vectPos[0] - vectCenter[0];
		vectSpd[1] = vectPos[1] - vectCenter[1];
		vectSpd[2] = vectPos[2] - vectCenter[2];
		normalizeVect(vectSpd);
		vectSpd[0] = vectCenter[0] - (32 * vectSpd[0]);
		vectSpd[1] = vectCenter[1] - (32 * vectSpd[1]);
		vectSpd[2] = vectCenter[2] - (32 * vectSpd[2]);

		camera.positionEye(vectSpd[0], vectSpd[1], vectSpd[2]);
		camera.aimAt(vectPos[0], vectPos[1], vectPos[2]);
		break;
	}
}

void printBoidsState() {
	int i = 1;

	for (std::vector<Boid>::iterator it = boids.begin(); it != boids.end(); it++) {
		(*it).printState(i);
		i++;
	}
}

void createRandomBoid() {
	float r;
	float center[3];
	
	getBoidsCenter(center);
	r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX / 10);
	r -= 5.0f;
	center[0] += r;
	r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX / 10);
	r -= 5.0f;
	center[1] += r;
	r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX / 10);
	r -= 5.0f;
	center[2] += r;

	boids.push_back(Boid(center[0], center[1], center[2], boss, boids, boids.size(), cone, obstacles));
}

void deleteRandomBoid() {
	int total, r;

	total = boids.size();
	if (total <= 1) {
		return;
	}

	r = rand() % total;
	boids.erase(boids.begin() + r);
}

void updateBoids() {
	int i;

	i = 0;
	for (std::vector<Boid>::iterator it = boids.begin(); it != boids.end(); it++) {
		(*it).update(boss, boids, i, cone, obstacles);
		i++;
	}
}

void draw() {
	//Draw the bounding box
	glLoadIdentity();
	camera.goToModelView();
	boundingBox.draw();
	//Draw the cone
	glLoadIdentity();
	camera.goToModelView();
	cone.draw();
	//Draw the boss boid
	glLoadIdentity();
	camera.goToModelView();
	boss.draw(drawMode);
	//Draw the boids
	for (std::vector<Boid>::iterator it = boids.begin(); it != boids.end(); it++) {
		glLoadIdentity();
		camera.goToModelView();
		(*it).draw(drawMode);
	}
	//Draw obstacles
	for (std::vector<Obstacle>::iterator it = obstacles.begin(); it != obstacles.end(); it++) {
		glLoadIdentity();
		camera.goToModelView();
		(*it).draw();
	}
}

int main(int argc, const char* argv[]) {
	GLFWwindow* window;

	if (!initGL(&window)) {
		return -1;
	}

	srand(time(NULL));

	boss.setSpeed(0.2f);
	boss.setYAngle(PI/2.0f);
	boss.setZAngle(PI);
	boss.setPosition(40.0f, 100.0f, 40.0f);

	boids.push_back(Boid(40.0f, 100.0f, 70.0f, boss, boids, boids.size(), cone, obstacles));

	createObstacles();

	positionCamera();

	while (!glfwWindowShouldClose(window) && running) {
		clearScreen();

		glfwPollEvents();

		if (!paused) {
			boss.rotateSpeedVectorManual(bossMovement);
			boss.update(boundingBox);
			updateBoids();
			positionCamera();
		}

		else if (runonce) {
			boss.rotateSpeedVectorManual(bossMovement);
			boss.update(boundingBox);
			updateBoids();
			positionCamera();
			boss.printState(0);
			printBoidsState();
			printf("\n");
			runonce = false;
		}

		draw();

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}
