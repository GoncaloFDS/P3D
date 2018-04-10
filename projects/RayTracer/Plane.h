#pragma once
#include "SceneObject.h"

class Plane : public SceneObject {
public:
	Plane(Vector3 a, Vector3 b, Vector3 c) : P1(a), P2(b), P3(c){};
	~Plane();
	Hit CalculateIntersection(Ray ray) override;
	Vector3 P1, P2, P3;
};

