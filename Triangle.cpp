#include "Triangle.h"

Triangle::~Triangle(){}

Hit Triangle::CalculateIntersection(Ray ray) {
	Vector3 edge1, edge2, tvec, pvec, qvec;
	double det, invDet;
	double t, u, v;

	edge1 = Vert2 - Vert1;
	edge2 = Vert3 - Vert1;

	pvec = Vector3::crossProduct(ray.Direction, edge2);

	det = edge1 * pvec;

	if (det < KEPSILON) // ray lies in plane of triangle
		return Hit(false);

	tvec = ray.Origin - Vert1;

	u = tvec * pvec;
	if (u < 0.0 || u > det)
		return Hit(false);

	qvec = tvec.cross(edge1);

	v = ray.Direction * qvec;
	if (v < 0.0 || u + v > det)
		return Hit(false);

	t = edge2 * qvec;
	invDet = 1.0 / det;
	t *= invDet;

	if (t < KEPSILON)
		return Hit(false);

	return Hit(ray.Origin + t * ray.Direction, true, edge1.cross(edge2).normalize(), t);
}
