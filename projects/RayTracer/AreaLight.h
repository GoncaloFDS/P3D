#pragma once
#include "Vector3.h"
#include "Light.h"

class AreaLight : public Light{
public:
	AreaLight() = default;
	AreaLight(Vector3 pos, Vector3 v2, Vector3 v3, Vector3 color, int SampleSize)
		: Light(pos, color, SampleSize), Va(v2), Vb(v3) {};
	~AreaLight() = default;

	Vector3 getPoint();

protected:
	Vector3 Va, Vb;
	
};

