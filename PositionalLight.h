#pragma once
#include "Vector3.h"
#include "projects/RayTracer/Color.h"

class PositionalLight
{
public:
	PositionalLight();
	~PositionalLight();
	Vector3 Position;
	Color Color;
};

