#include "Sphere.h"


Sphere::Sphere()
{

}


Sphere::~Sphere()
{
}

Hit Sphere::CalculateIntersection(Ray ray)
{
	double t;
	Vector3 temp = ray.Origin - center;
	double a = ray.Direction * ray.Direction; // length of ray.Direction. should be 1 
	double b = 2.0 * temp * ray.Direction;
	double c = temp * temp - radius	* radius;
	double disc = b * b - 4.0 * a * c;

	if(disc < 0.0)
		return Hit(false);

	double e = sqrt(disc);
	double denom = 2.0 * a;
	t = (-b -e) / denom; // root 1

	if( t > KEPSILON) 
		return Hit(ray.Origin + t * ray.Direction, true, (temp + t * ray.Direction)/radius, t);
	
	t = (-b + e) / denom; //root 2

	if(t > KEPSILON) 
		return Hit(ray.Origin + t * ray.Direction, true, (temp + t * ray.Direction)/radius, t);

	return Hit(false);
}
