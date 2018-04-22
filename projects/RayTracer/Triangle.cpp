#include "Triangle.h"

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
	BBox bbox;
	Vector3 maxP(FLT_MIN);
	Vector3 minP(FLT_MAX);

	for (auto v : Verts) {
		if (v.x() > maxP.x())
			maxP.x() = v.x();
		if (v.y() > maxP.y())
			maxP.y() = v.y();
		if (v.z() > maxP.z())
			maxP.z() = v.z();

		if (bbox.x0 < minP.x())
			minP.x() = bbox.x0;
		if (v.y() < minP.y())
			minP.y() = v.y();
		if (v.z() < minP.z())
			minP.z() = v.z();
	}

	return bbox;
}
