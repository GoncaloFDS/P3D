#pragma once
#include "Camera.h"

class ThinLens : public Camera
{
public:
	virtual Ray calculatePrimaryRay(int x, int y) override;
	float radius;
	float f = -1; // distance to focal plane
	float d = -1; // distance to view plane
	float apperture = -1;
	ThinLens() = default;
	~ThinLens() = default;
	void computeParams() override;

};

