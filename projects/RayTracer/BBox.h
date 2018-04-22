#pragma once
#include <float.h>
#include "Hit.h"
#include "Ray.h"

class BBox
{
public:
	BBox() = default;
	~BBox() = default;
	float x0 = FLT_MAX, y0 = FLT_MAX, z0 = FLT_MAX;
	float x1 = FLT_MIN, y1 = FLT_MIN, z1 = FLT_MIN;

	Hit calculateIntersection(Ray ray);
};

