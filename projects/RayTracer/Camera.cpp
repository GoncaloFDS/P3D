#include "Camera.h"


Camera::Camera()
{
}


Camera::~Camera()
{
}

float Camera::GetResX()
{
	return Resolution.coordinates[0];
}

float Camera::GetResY()
{
	return Resolution.coordinates[1];
}

Ray Camera::CalculatePrimaryRay(int x, int y)
{
	float ux = ((x +0.5)/ GetResX())*Width;
	float vy = ((y+0.5) / GetResY())*Height;

	Vector3 eyeAt = (Eye - At);
	float df = (eyeAt).magnitude();

	Ray ray;
	ray.Origin = Eye;
	ray.Direction = -df * Ze + Height*(y/GetResY() - 0.5f)*Ye + Width*(x/GetResX() - 0.5f)*Xe;
	ray.Direction = ray.Direction.normalize();
	return ray;
}

void Camera::CalculateOtherStuff()
{
	Vector3 eyeAt = (Eye - At);
	float df = (eyeAt).magnitude();
	Height = 2 * df * tanf(Fovy / 2);
	Width = (GetResX() / GetResY()) * Height;
	Ze = eyeAt.normalize();
	Xe = Vector3::crossProduct(Up, Ze).normalize();
	Ye = Vector3::crossProduct(Ze, Xe);

}
