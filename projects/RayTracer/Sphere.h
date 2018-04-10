#pragma once

#include "SceneObject.h"
#include "Vector3.h"
#include "Material.h"

class Sphere : public SceneObject
{
public:
	Sphere();
	~Sphere();
	Vector3 center;
	float radius;

	Hit CalculateIntersection(Ray ray) override;
};

