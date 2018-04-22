#pragma once
#include "Vector3.h"

class Ray
{
public:
	Ray();
	Ray(Vector3 origin, Vector3 direction) : O(origin), Dir(direction) {}
	~Ray();
	Vector3 O;
	Vector3 Dir;

};

