#include "ThinLens.h"



// Ray ThinLens::CalculatePrimaryRay(int x, int y)
// {
// 	float ux = ((x) / GetResX())*Width;
// 	float vy = ((y) / GetResY())*Height;
// 	Vector3 eyeAt = (Eye - At);
// 	float df = (eyeAt).magnitude();
// 	Ray ray;
// 	Vector2 P;
// 	float f = 3.5f;
// 	float d = eyeAt.magnitude();
// 	float apperture = 5;
// 
// 	P.coordinates[0] = x * f/d;
// 	P.coordinates[1] = y * f/d;
// 	//randoms
// 	Vector3 randomPoint;
// 	do {
// 		float xEps = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
// 		float yEps = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
// 		xEps = ((xEps * 2 - 1) * radius) / GetResX()*Width;
// 		yEps = ((yEps * 2 - 1) * radius) / GetResY()*Height;
// 		randomPoint = (xEps * Xe + yEps * -Ye);
// 	} while (randomPoint.quadrance() > radius * radius);
// 
// 	ray.Origin = Eye + randomPoint;
// 	ray.Direction = ((P.coordinates[0] - ray.Origin.x()) * Xe + (P.coordinates[1] - ray.Origin.y()) * -Ye - f * Ze).normalize();
// 	return ray;
// }

Ray ThinLens::CalculatePrimaryRay(int x, int y)
{	
	Ray ray;
	float apperture = 5;
	radius = 0.1;
	//ray Origin 
	Vector2 pixelP;
	Vector2 diskP;
	Vector2 randomP = {
		static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
		static_cast <float> (rand()) / static_cast <float> (RAND_MAX)
	}; // sample point in [0, 1] x [0, 1]

	pixelP.x() = Width / GetResX() * (x - GetResX() / 2.0f + randomP.x());
	pixelP.y() = Height / GetResY() * (y - GetResY() / 2.0f + randomP.y());
	
	do {	
		diskP = {
			static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
			static_cast <float> (rand()) / static_cast <float> (RAND_MAX)
		}; // sample point in [0, 1] x [0, 1]
	} while (diskP.quadrance() > 1);
	

	Vector2 lensP = diskP * radius;

	ray.Origin = Eye + lensP.x() * Xe + lensP.y() * Ye;

	//ray direction
	Vector2 p;
	p.x() =	pixelP.x() * f / d;
	p.y() =	pixelP.y() * f / d;

	Vector3 dir = (p.x() - lensP.x()) * Xe + (p.y() - lensP.y()) * Ye - f * Ze;
	ray.Direction = dir.normalize();
	return ray;
}