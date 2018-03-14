#include "Plane.h"


Plane::~Plane(){}

Hit Plane::CalculateIntersection(Ray ray) {
	Vector3 planeNormal(Vector3::crossProduct((P2-P1), (P3-P1)));
	if(planeNormal * ray.Direction == 0)
		return Hit(false);

	double t = (P1 - ray.Origin) * planeNormal / (ray.Direction * planeNormal);
	if (t > KEPSILON) {
		return Hit(ray.Origin + t * ray.Direction, true, planeNormal, t);
	}
	return Hit(false);
}

