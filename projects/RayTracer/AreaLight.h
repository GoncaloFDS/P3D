#pragma once
#include "Vector3.h"
#include "Light.h"

class AreaLight : public Light{
public:
	AreaLight();
	AreaLight(Vector3 pos, Vector3 v2, Vector3 v3, Vector3 color, int SampleSize)
		: Light(pos, color, SampleSize), Va(v2), Vb(v3) {};
	~AreaLight() = default;

	Vector3 GetPoint();

protected:
	Vector3 Va, Vb;
	
};

