#include "Plane.h"

Hit Plane::calculateIntersection(Ray ray) {
	Vector3 planeNormal(Vector3::crossProduct((P2-P1), (P3-P1)));
	if(planeNormal * ray.Dir == 0)
		return Hit(false);

	double t = (P1 - ray.O) * planeNormal / (ray.Dir * planeNormal);
	if (t > KEPSILON) {
		return Hit(ray.O + t * ray.Dir, true, planeNormal.normalize(), t);
	}
	return Hit(false);
}

BBox Plane::getBoundingBox()
{
	return BBox();
}

