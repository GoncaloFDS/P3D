#pragma once
#include "Camera.h"

class ThinLens : public Camera
{
public:
	virtual Ray calculatePrimaryRay(int x, int y) override;
	float radius;
	float f; // distance to focal plane
	float d; // distance to view plane
	float apperture;
	ThinLens() = default;
	~ThinLens() = default;
	void computeParams() override;

};

