#pragma once
#include "Vector3.h"
#include "projects/RayTracer/Color.h"

class PointLight
{
public:
	PointLight();
	~PointLight();
	Vector3 Position;
	Vector3 Color;
};

