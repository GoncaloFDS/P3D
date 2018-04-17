#pragma once
#include "c:\Users\Haru\Desktop\P3D_DOF\P3D\projects\RayTracer\Camera.h"
class ThinLens :
	public Camera
{
public:
	virtual Ray CalculatePrimaryRay(int x, int y) override;
	float radius = 2;
	float f; // distance to focal plane
	float d; // distance to view plane
	float zoom;
	float samples;
	ThinLens();
	~ThinLens();
};

