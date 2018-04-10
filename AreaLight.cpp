#include "AreaLight.h"



AreaLight::AreaLight()
{
}


AreaLight::~AreaLight()
{
}

Vector3 AreaLight::getRandomPoint() {
	float pEps = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float qEps = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	return c + pEps * a + qEps * b;
}
