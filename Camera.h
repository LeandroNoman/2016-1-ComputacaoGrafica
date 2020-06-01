#pragma once

class Camera {
public:
	void goToModelView();
	void positionEye(double x, double y, double z);
	void aimAt(double x, double y, double z);

private:
	double eyePosition[3] = { 0., 50., 0. };
	double lookAt[3] = { 0., 0., 0. };
	double upVector[3] = { 0., 1., 0. };

};