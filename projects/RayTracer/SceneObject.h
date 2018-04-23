#pragma once
#include "Ray.h"
#include "Hit.h"
#include "Material.h"
#include "BBox.h"

#define KEPSILON 0.0001f

class SceneObject
{
public:
	Material *material;

	SceneObject() = default;
	~SceneObject() = default;
	
	virtual Hit calculateIntersection(Ray ray) = 0;
	virtual BBox getBoundingBox() = 0;
};

