#include "Grid.h"
#include <float.h>


Vector3 Grid::getMinCoords()
{
	BBox bbox;
	Vector3 minP(FLT_MAX);
	
// 	for (int i = 0; i < Objects->size(); i++) {
// 		bbox = Objects->at(i)->getBoundingBox();
// 
// 		if (bbox.x0 < minP.x())
// 			minP.x() = bbox.x0;
// 		if (bbox.y0 < minP.y())
// 			minP.y() = bbox.y0;
// 		if (bbox.z0 < minP.z())
// 			minP.z() = bbox.z0;
// 	}

	minP.x() -= KEPSILON;
	minP.y() -= KEPSILON;
	minP.z() -= KEPSILON;

	return minP;
}

Vector3 Grid::getMaxCoords()
{
	BBox bbox;
	Vector3 maxP(FLT_MIN);

// 	for (int i = 0; i < Objects->size(); i++) {
// 		bbox = Objects->at(i)->getBoundingBox();
// 
// 		if (bbox.x1 > maxP.x())
// 			maxP.x() = bbox.x1;
// 		if (bbox.y1 > maxP.y())
// 			maxP.y() = bbox.y1;
// 		if (bbox.z1 > maxP.z())
// 			maxP.z() = bbox.z1;
// 	}

	maxP.x() += KEPSILON;
	maxP.y() += KEPSILON;
	maxP.z() += KEPSILON;

	return maxP;
}
