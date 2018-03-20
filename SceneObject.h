#pragma once
#include "Ray.h"
#include "Hit.h"
#include "Material.h"

#define KEPSILON 0.0001f

class SceneObject
{
public:
	SceneObject();
	~SceneObject();
	Material material;
	virtual Hit CalculateIntersection(Ray ray);
};

