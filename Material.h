#pragma once
#include "projects/RayTracer/Color.h"

class Material
{
public:
	Material();
	~Material();
	Vector3 color;
	float Kd;
	float Ks;
	float shininess;
	float T;
	float refractionIndex;
};

