#pragma once
#include "Vector3.h"

class Hit
{
public:
	Hit();
	Hit(bool hasCollided) : HasCollided(hasCollided){};
	Hit(Vector3 location, bool hasCollided, Vector3 normal, double t) : Location(location), HasCollided(hasCollided), Normal(normal) , T(t){};
	~Hit();

	Vector3 Location;
	bool HasCollided = false;
	Vector3 Normal;
	double T;
};

