#pragma once
#include "Camera.h"

class ThinLens :
	public Camera
{
public:
	virtual Ray CalculatePrimaryRay(int x, int y) override;
	float radius;
	float f; // distance to focal plane
	float d; // distance to view plane
	float zoom;
	float samples;
	ThinLens() = default;
	~ThinLens() = default;
};

