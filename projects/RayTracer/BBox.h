#pragma once
#include <float.h>
#include "Hit.h"
#include "Ray.h"

class BBox
{
public:
	BBox() = default;
	~BBox() = default;
	BBox::BBox(float _x0, float _x1,
		float _y0, float _y1,
		float _z0, float _z1)
		: x0(_x0), x1(_x1), y0(_y0), y1(_y1), z0(_z0), z1(_z1) {}

	float x0 = FLT_MAX, y0 = FLT_MAX, z0 = FLT_MAX;
	float x1 = FLT_MIN, y1 = FLT_MIN, z1 = FLT_MIN;

	Hit calculateIntersection(Ray ray);
	bool inside(Vector3 O);
};

