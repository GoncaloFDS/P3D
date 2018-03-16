#pragma once
#include "Vector3.h"

class Color : Vector3 {
public:
	Color();
	~Color();
	float r = 0;
	float g = 0;
	float b = 0;
	Color(float _r, float _g, float _b);
	Color(Vector3 vec3);
	
};

