#include "Camera.h"

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
	Vector2 pixelP;
	Vector2 randomP = { 0.5, 0.5
		//static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
		//static_cast <float> (rand()) / static_cast <float> (RAND_MAX)
	}; // sample point in [0, 1] x [0, 1]

	pixelP.x() = Width / GetResX() * (x - GetResX() / 2.0f + randomP.x());
	pixelP.y() = Height / GetResY() * (y - GetResY() / 2.0f + randomP.y());

	float df = (Eye-At).magnitude();

	Ray ray;
	ray.Origin = Eye;
	ray.Direction = pixelP.x() * Xe + pixelP.y() * Ye - df * Ze;
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
