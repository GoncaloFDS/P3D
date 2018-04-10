#pragma once
#include "Vector3.h"

class AreaLight {
public:
	AreaLight();
	AreaLight(Vector3 v1, Vector3 v2, Vector3 v3) : c(v1), a(v2), b(v3) {};
	~AreaLight();

	Vector3 c, a, b;
	
	Vector3 getRandomPoint();
};

