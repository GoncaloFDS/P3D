#include "AreaLight.h"

Vector3 AreaLight::getPoint() {
	float pEps = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float qEps = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	return Position + pEps * Va + qEps * Vb;
}
