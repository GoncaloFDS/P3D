#pragma once
#include "Vector3.h"

class PointLight 
{
public:
	PointLight() = default;
	~PointLight() = default;
	Vector3 Position;
	Vector3 Color;
};

