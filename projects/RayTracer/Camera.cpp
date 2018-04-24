#include "Camera.h"

float Camera::getResX()
{
	return Resolution.coordinates[0];
}

float Camera::getResY()
{
	return Resolution.coordinates[1];
}

Ray Camera::calculatePrimaryRay(int x, int y)
{	
	Vector2 pixelP, randomP;
	if (AAenabled)
		randomP = { // sample point in [0, 1] x [0, 1]
		static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
		static_cast <float> (rand()) / static_cast <float> (RAND_MAX)
	};
	else
		randomP = { 0.5, 0.5 };

	pixelP.x() = Width / getResX() * (x - getResX() / 2.0f + randomP.x());
	pixelP.y() = Height / getResY() * (y - getResY() / 2.0f + randomP.y());

	float df = (Eye-At).magnitude();

	Ray ray;
	ray.O = Eye;
	ray.Dir = pixelP.x() * Xe + pixelP.y() * Ye - df * Ze;
	ray.Dir = ray.Dir.normalize();
	return ray;
}

void Camera::computeParams()
{
	Vector3 eyeAt = Eye - At;
	float df = (eyeAt).magnitude();
	Height = 2 * df * tanf(Fovy / 2);
	Width = (getResX() / getResY()) * Height;
	Ze = eyeAt.normalize();
	Xe = Vector3::crossProduct(Up, Ze).normalize();
	Ye = Vector3::crossProduct(Ze, Xe);

}
