#include "Sphere.h"

Hit Sphere::calculateIntersection(Ray ray)
{
	double t;
	Vector3 temp = ray.O - center;
	double a = ray.Dir * ray.Dir; // length of ray.Direction. should be 1 
	double b = 2.0 * temp * ray.Dir;
	double c = temp * temp - radius	* radius;
	double disc = b * b - 4.0 * a * c;

	if(disc < 0.0)
		return Hit(false);

	double e = sqrt(disc);
	double denom = 2.0 * a;
	t = (-b -e) / denom; // root 1

	if( t > KEPSILON) 
		return Hit(ray.O + t * ray.Dir, true, (temp + t * ray.Dir)/radius, t);
	
	t = (-b + e) / denom; //root 2

	if(t > KEPSILON) 
		return Hit(ray.O + t * ray.Dir, true, (temp + t * ray.Dir)/radius, t);

	return Hit(false);
}

BBox Sphere::getBoundingBox()
{
	BBox bbox;
	bbox.x0 = center.x() - radius;
	bbox.y0 = center.y() - radius;
	bbox.z0 = center.z() - radius;
	bbox.x1 = center.x() + radius;
	bbox.y1 = center.y() + radius;
	bbox.z1 = center.z() + radius;
	return bbox;
}

