#pragma once
#include "Vector3.h"

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
	bool isTranslucid;
	float refractionIndex;
};

