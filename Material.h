#pragma once
#include "projects/RayTracer/Color.h"

class Material
{
public:
	Material();
	~Material();
	Color color;
	float Kd;
	float Ks;
	float shininess;
	float T;
	float refractionIndex;
};

