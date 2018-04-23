#include "Grid.h"
#include <float.h>

inline float clamp(float x, float min, float max) {
	return (x < min ? min : (x > max ? max : x));
}

void Grid::setupCells()
{
	Vector3 p0 = getMinCoords();
	Vector3 p1 = getMaxCoords();

	//store them in a bounding box

	Bbox.x0 = p0.x(); Bbox.y0 = p0.y(); Bbox.z0 = p0.z();
	Bbox.x1 = p1.x(); Bbox.y1 = p1.y(); Bbox.z1 = p1.z();

	//compute the number of cells in the xyz directions
	int numObjs = Objects->size();
	float wx = p1.x() - p0.x();
	float wy = p1.y() - p0.y();
	float wz = p1.z() - p0.z();
	float multiplier = 2.0;			//8 times more cells than objectsS
	float s = pow(wx * wy * wz / numObjs, 0.333333);
	Nx = multiplier * wx / s + 1;
	Ny = multiplier * wy / s + 1;
	Nz = multiplier * wz / s + 1;

	//set up the array of cells with null pointers

	int numCells = Nx * Ny * Nz;
	//Cells.reserve(numObjs);
 	
	Cells = new std::vector<std::vector<SceneObject *>>(numCells, std::vector<SceneObject*>());
	//set up a temp array to hold the number of objects stored in each cell

	std::vector<int> counts;
	counts.reserve(numCells);

	for (int i = 0; i < numCells; i++)
		counts.push_back(0);

	//put objects into the cells
	BBox objBox;
	int index;

	for (int i = 0; i < numObjs; i++){
		objBox = Objects->at(i)->getBoundingBox();
		//compute the cell indices for the corners of the bounding box of the obj

		int ixmin = clamp((objBox.x0 - p0.x()) * Nx / (p1.x() - p0.x()), 0, Nx - 1);
		int iymin = clamp((objBox.y0 - p0.y()) * Ny / (p1.y() - p0.y()), 0, Ny - 1);
		int izmin = clamp((objBox.z0 - p0.z()) * Nz / (p1.z() - p0.z()), 0, Nz - 1);
		int ixmax = clamp((objBox.x1 - p0.x()) * Nx / (p1.x() - p0.x()), 0, Nx - 1);
		int iymax = clamp((objBox.y1 - p0.y()) * Ny / (p1.y() - p0.y()), 0, Ny - 1);
		int izmax = clamp((objBox.z1 - p0.z()) * Nz / (p1.z() - p0.z()), 0, Nz - 1);

		//add the objects to the cells
		for (int iz = izmin; iz <= izmax; iz++) {
			for (int iy = iymin; iy <= iymax; iy++) {
				for (int ix = ixmin; ix <= ixmax; ix++) {
					index = ix + Nx * iy + Nx * Ny* iz;
					Cells->at(index).push_back(Objects->at(i));
					counts[index] += 1;
				}
			}
		}
	}
	//Objects->erase(Objects->begin(), Objects->end());
	//counts.erase(counts.begin(), counts.end());
}


double Grid::cellClossestHit(std::vector<SceneObject *> cellObjs, Ray& ray, double Tmin, Hit &hit) const
{
	for (auto obj : cellObjs) {
		Hit tempHit = obj->calculateIntersection(ray);
		if ((tempHit.HasCollided && tempHit.T < Tmin)) {
			Tmin = tempHit.T;
			hit = tempHit;
			hit.Mat = obj->material;
		}
	}
	return Tmin;
}

Hit Grid::hit(Ray& ray)
{
	double ox = ray.O.x();
	double oy = ray.O.y();
	double oz = ray.O.z();
	double dx = ray.Dir.x();
	double dy = ray.Dir.y();
	double dz = ray.Dir.z();

	double x0 = Bbox.x0;
	double y0 = Bbox.y0;
	double z0 = Bbox.z0;
	double x1 = Bbox.x1;
	double y1 = Bbox.y1;
	double z1 = Bbox.z1;

	double txMin, tyMin, tzMin;
	double txMax, tyMax, tzMax;

	// the following code includes modifications from Shirley and Morley (2003)

	double a = 1.0 / dx;
	if (a >= 0) {
		txMin = (x0 - ox) * a;
		txMax = (x1 - ox) * a;
	}
	else {
		txMin = (x1 - ox) * a;
		txMax = (x0 - ox) * a;
	}

	double b = 1.0 / dy;
	if (b >= 0) {
		tyMin = (y0 - oy) * b;
		tyMax = (y1 - oy) * b;
	}
	else {
		tyMin = (y1 - oy) * b;
		tyMax = (y0 - oy) * b;
	}

	double c = 1.0 / dz;
	if (c >= 0) {
		tzMin = (z0 - oz) * c;
		tzMax = (z1 - oz) * c;
	}
	else {
		tzMin = (z1 - oz) * c;
		tzMax = (z0 - oz) * c;
	}

	double t0, t1;

	if (txMin > tyMin)
		t0 = txMin;
	else
		t0 = tyMin;

	if (tzMin > t0)
		t0 = tzMin;

	if (txMax < tyMax)
		t1 = txMax;
	else
		t1 = tyMax;

	if (tzMax < t1)
		t1 = tzMax;

	if (t0 > t1)
		return(false);


	// initial cell coordinates

	int ix, iy, iz;

	if (Bbox.inside(ray.O)) {  			   // does the ray start inside the grid?
		ix = clamp((ox - x0) * Nx / (x1 - x0), 0, Nx - 1);
		iy = clamp((oy - y0) * Ny / (y1 - y0), 0, Ny - 1);
		iz = clamp((oz - z0) * Nz / (z1 - z0), 0, Nz - 1);
	}
	else {
		Vector3 p = ray.O + t0 * ray.Dir;  // initial hit point with grid's bounding box
		ix = clamp((p.x() - x0) * Nx / (x1 - x0), 0, Nx - 1);
		iy = clamp((p.y() - y0) * Ny / (y1 - y0), 0, Ny - 1);
		iz = clamp((p.z() - z0) * Nz / (z1 - z0), 0, Nz - 1);
	}

	// ray parameter increments per cell in the x, y, and z directions

	double dtx = (txMax - txMin) / Nx;
	double dty = (tyMax - tyMin) / Ny;
	double dtz = (tzMax - tzMin) / Nz;

	double txNext, tyNext, tzNext;
	int	ixStep, iyStep, izStep;
	int ixStop, iyStop, izStop;

	if (dx > 0) {
		txNext = txMin + (ix + 1) * dtx;
		ixStep = +1;
		ixStop = Nx;
	}
	else {
		txNext = txMin + (Nx - ix) * dtx;
		ixStep = -1;
		ixStop = -1;
	}

	if (dx == 0.0) {
		txNext = DBL_MAX;
		ixStep = -1;
		ixStop = -1;
	}

	if (dy > 0) {
		tyNext = tyMin + (iy + 1) * dty;
		iyStep = +1;
		iyStop = Ny;
	}
	else {
		tyNext = tyMin + (Ny - iy) * dty;
		iyStep = -1;
		iyStop = -1;
	}

	if (dy == 0.0) {
		tyNext = DBL_MAX;
		iyStep = -1;
		iyStop = -1;
	}

	if (dz > 0) {
		tzNext = tzMin + (iz + 1) * dtz;
		izStep = +1;
		izStop = Nz;
	}
	else {
		tzNext = tzMin + (Nz - iz) * dtz;
		izStep = -1;
		izStop = -1;
	}

	if (dz == 0.0) {
		tzNext = DBL_MAX;
		izStep = -1;
		izStop = -1;
	}


	// traverse the grid

	while (true) {
		double Tmin = DBL_MAX;
		Hit hit;
		auto cellObjs = Cells->at( ix + Nx * iy + Nx * Ny * iz);
		if (txNext < tyNext && txNext < tzNext) {
			Tmin = cellClossestHit(cellObjs, ray, Tmin, hit);

			if(hit.T < tzNext) 
				return hit;
			
			txNext += dtx;
			ix += ixStep;

			if (ix == ixStop)
				return Hit(false);
		}
		else {
			if(tyNext < tzNext) {
				Tmin = cellClossestHit(cellObjs, ray, Tmin, hit);
				if (hit.T < tyNext)
					return hit;

				tyNext += dty;
				iy += iyStep;

				if (iy == iyStop)
					return Hit(false);
			} 
			else {
				Tmin = cellClossestHit(cellObjs, ray, Tmin, hit);
				if (hit.T < tzNext)
					return hit;

				tzNext += dtz;
				iz += izStep;

				if (iz == izStop)
					return Hit(false);
			}
		}
	}

}

Vector3 Grid::getMinCoords()
{
	BBox bbox;
	Vector3 minP(FLT_MAX);
	
	for (auto & Object : *Objects) {
		bbox = Object->getBoundingBox();

		if (bbox.x0 < minP.x())
			minP.x() = bbox.x0;
		if (bbox.y0 < minP.y())
			minP.y() = bbox.y0;
		if (bbox.z0 < minP.z())
			minP.z() = bbox.z0;
	}

	minP.x() -= KEPSILON;
	minP.y() -= KEPSILON;
	minP.z() -= KEPSILON;

	return minP;
}

Vector3 Grid::getMaxCoords()
{
	BBox bbox;
	Vector3 maxP(FLT_MIN);

	for (auto & Object : *Objects) {
		bbox = Object->getBoundingBox();

		if (bbox.x1 > maxP.x())
			maxP.x() = bbox.x1;
		if (bbox.y1 > maxP.y())
			maxP.y() = bbox.y1;
		if (bbox.z1 > maxP.z())
			maxP.z() = bbox.z1;
	}

	maxP.x() += KEPSILON;
	maxP.y() += KEPSILON;
	maxP.z() += KEPSILON;

	return maxP;
}
