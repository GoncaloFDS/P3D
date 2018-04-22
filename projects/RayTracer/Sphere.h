#pragma once

#include "SceneObject.h"
#include "Vector3.h"
#include "Material.h"

class Sphere : public SceneObject
{
public:
	Sphere() = default;
	~Sphere() = default;
	Vector3 center;
	float radius;

	Hit calculateIntersection(Ray ray) override;
	BBox getBoundingBox();
};

