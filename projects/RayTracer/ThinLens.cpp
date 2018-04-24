#include "ThinLens.h"


Ray ThinLens::calculatePrimaryRay(int x, int y)
{	
	if (!DOFenabled || apperture == -1 || f == -1){ 
		return Camera::calculatePrimaryRay(x, y);
	}

	Ray ray;
	//ray Origin 
	Vector2 pixelP, diskP, randomP;
	if (AAenabled)
		randomP = { // sample point in [0, 1] x [0, 1]
			static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
			static_cast <float> (rand()) / static_cast <float> (RAND_MAX)
		}; 
	else
		randomP = { 0.5, 0.5 };

	pixelP.x() = Width / getResX() * (x - getResX() / 2.0f + randomP.x());
	pixelP.y() = Height / getResY() * (y - getResY() / 2.0f + randomP.y());
	
	do {	
		diskP = {
			static_cast <float> (rand()) / static_cast <float> (RAND_MAX),
			static_cast <float> (rand()) / static_cast <float> (RAND_MAX)
		}; // sample point in [0, 1] x [0, 1]
	} while (diskP.quadrance() > 1);
	

	Vector2 lensP = diskP * radius * apperture; //apperture used to open or close the lens

	ray.O = Eye + lensP.x() * Xe + lensP.y() * Ye;

	//ray direction
	Vector2 p;
	p.x() =	pixelP.x() * f / d;
	p.y() =	pixelP.y() * f / d;

	Vector3 dir = (p.x() - lensP.x()) * Xe + (p.y() - lensP.y()) * Ye - f * Ze;
	ray.Dir = dir.normalize();
	return ray;
}


void ThinLens::computeParams()
{
	Camera::computeParams();
	d = (Eye - At).magnitude();
	if (f == -1 || apperture == -1)
		DOFenabled = false;
}
