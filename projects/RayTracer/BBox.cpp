#include "BBox.h"

Hit BBox::calculateIntersection(Ray ray)
{	
	double ox = ray.O.x(), oy = ray.O.y(), oz = ray.O.z();
	double dx = ray.Dir.x(), dy = ray.Dir.y(), dz = ray.Dir.z();

	double txMin, tyMin, tzMin;
	double txMax, tyMax, tzMax;

	double a = 1.f / dx;

	if (a >= 0) {
		txMin = (x0 - ox) * a;
		txMax = (x1 - ox) * a;
	}
	else {
		txMin = (x1 - ox) * a;
		txMax = (x0 - ox) * a;
	}

	double b = 1.f / dy;
	if (b >= 0) {
		tyMin = (y0 - oy) * b;
		tyMax = (y1 - oy) * b;
	}
	else {
		tyMin = (y1 - oy) * b;
		tyMax = (y0 - oy) * b;
	}

	double c = 1.f / dz;
	if (c >= 0) {
		tzMin = (z0 - oz) * c;
		tzMax = (z1 - oz) * c;
	}
	else {
		tzMin = (z1 - oz) * c;
		tzMax = (z0 - oz) * c;
	}

	double t0, t1;

	//find largest entering t value
	if (txMin > tyMin)
		t0 = txMin;
	else
		t0 = tyMin;
	if (tzMin > 0)
		t0 = tzMin;

	//find smallest exiting t value;
	if (txMax < tyMax)
		t1 = txMax;
	else
		t1 = tyMax;
	if (tzMax < t1)
		t1 = tzMax;

	return Hit(t0 < t1 && t1 > 0.0001);
}

