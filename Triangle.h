#pragma once
#include "SceneObject.h"

class Triangle : public SceneObject {
public:
	Triangle(Vector3 p1, Vector3 p2, Vector3 p3) : Vert1(p1), Vert2(p2), Vert3(p3) {};
	~Triangle();

	Vector3 Vert1, Vert2, Vert3;
	Hit CalculateIntersection(Ray ray) override;
};

