#include "camera.h"
#include "matrix.h"
using namespace std;

void OrthographicCamera::glInit(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w > h)
		glOrtho(-size / 2.0, size / 2.0, -size*(float)h / (float)w / 2.0, size*(float)h / (float)w / 2.0, 0.5, 40.0);
	else
		glOrtho(-size*(float)w / (float)h / 2.0, size*(float)w / (float)h / 2.0, -size / 2.0, size / 2.0, 0.5, 40.0);
}

void OrthographicCamera::glPlaceCamera(void)
{
	gluLookAt(center.x(), center.y(), center.z(),
		center.x() + direction.x(), center.y() + direction.y(), center.z() + direction.z(),
		up.x(), up.y(), up.z());
}

void OrthographicCamera::dollyCamera(float dist)
{
	center += direction*dist;
}

void OrthographicCamera::truckCamera(float dx, float dy)
{
	Vec3f horizontal;
	Vec3f::Cross3(horizontal, direction, up);
	horizontal.Normalize();

	Vec3f screenUp;
	Vec3f::Cross3(screenUp, horizontal, direction);

	center += horizontal*dx + screenUp*dy;
}

void OrthographicCamera::rotateCamera(float rx, float ry)
{
	Vec3f horizontal;
	Vec3f::Cross3(horizontal, direction, up);
	horizontal.Normalize();
	float tiltAngle = acos(up.Dot3(direction));
	if (tiltAngle - ry > 3.13)
		ry = tiltAngle - 3.13;
	else if (tiltAngle - ry < 0.01)
		ry = tiltAngle - 0.01;

	Matrix rotMat = Matrix::MakeAxisRotation(up, rx);
	rotMat *= Matrix::MakeAxisRotation(horizontal, ry);

	rotMat.Transform(center);
	rotMat.TransformDirection(direction);
}

void PerspectiveCamera::glInit(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(angle*180.0 / 3.14159, (float)w / float(h), 0.5, 40.0);
}

void PerspectiveCamera::glPlaceCamera(void)
{
	gluLookAt(center.x(), center.y(), center.z(),
		center.x() + direction.x(), center.y() + direction.y(), center.z() + direction.z(),
		up.x(), up.y(), up.z());
}

void PerspectiveCamera::dollyCamera(float dist)
{
	center += direction*dist;
}

void PerspectiveCamera::truckCamera(float dx, float dy)
{
	Vec3f horizontal;
	Vec3f::Cross3(horizontal, direction, up);
	horizontal.Normalize();

	Vec3f screenUp;
	Vec3f::Cross3(screenUp, horizontal, direction);

	center += horizontal*dx + screenUp*dy;
}

void PerspectiveCamera::rotateCamera(float rx, float ry)
{
	Vec3f horizontal;
	Vec3f::Cross3(horizontal, direction, up);
	horizontal.Normalize();
	float tiltAngle = acos(up.Dot3(direction));
	if (tiltAngle - ry > 3.13)
		ry = tiltAngle - 3.13;
	else if (tiltAngle - ry < 0.01)
		ry = tiltAngle - 0.01;

	Matrix rotMat = Matrix::MakeAxisRotation(up, rx);
	rotMat *= Matrix::MakeAxisRotation(horizontal, ry);

	rotMat.Transform(center);
	rotMat.TransformDirection(direction);
	direction.Normalize();
}
