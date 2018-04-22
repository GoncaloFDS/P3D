#pragma once
#include "SceneObject.h"

class Triangle : public SceneObject {
public:
	Triangle(Vector3 p1, Vector3 p2, Vector3 p3) {
		Verts[0] = p1;
		Verts[1] = p2;
		Verts[2] = p3;
	};
	~Triangle();

	Vector3 Verts[3];

	Hit calculateIntersection(Ray ray) override;

	BBox getBoundingBox() override;
};

