#include "Triangle.h"
#include <stdlib.h>
#include <algorithm>    // std::min

Triangle::~Triangle(){}

Hit Triangle::calculateIntersection(Ray ray) {
	Vector3 edge1, edge2, tvec, pvec, qvec;
	double det, invDet;
	double t, u, v;

	edge1 = Verts[1] - Verts[0];
	edge2 = Verts[2] - Verts[0];

	pvec = Vector3::crossProduct(ray.Dir, edge2);

	det = edge1 * pvec;

	if (det < 0) // ray lies in plane of triangle
		return Hit(false);

	tvec = ray.O - Verts[0];

	u = tvec * pvec;
	if (u < 0.0 || u > det)
		return Hit(false);

	qvec = tvec.cross(edge1);

	v = ray.Dir * qvec;
	if (v < 0.0 || u + v > det)
		return Hit(false);

	t = edge2 * qvec;
	invDet = 1.0 / det;
	t *= invDet;

	if (t < KEPSILON)
		return Hit(false);

	return Hit(ray.O + t * ray.Dir, true, edge1.cross(edge2).normalize(), t);
}

BBox Triangle::getBoundingBox()
{	

	return(BBox(std::min(std::min(Verts[0].x(), Verts[1].x()), Verts[2].x()) - KEPSILON, std::max(std::max(Verts[0].x(), Verts[1].x()), Verts[2].x()) + KEPSILON,
		std::min(std::min(Verts[0].y(), Verts[1].y()), Verts[2].y()) - KEPSILON, std::max(std::max(Verts[0].y(), Verts[1].y()), Verts[2].y()) + KEPSILON,
		std::min(std::min(Verts[0].z(), Verts[1].z()), Verts[2].z()) - KEPSILON, std::max(std::max(Verts[0].z(), Verts[1].z()), Verts[2].z()) + KEPSILON));


}
