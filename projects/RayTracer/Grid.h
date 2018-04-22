#pragma once
#include "BBox.h"
#include <vector>
#include "Scene.h"


class Grid
{
public:
	Grid() = default;
	~Grid() = default;
	//Grid(std::vector<SceneObject*>* objs) : Objects(objs) {}

	virtual BBox getBoundingBox();
	void setupCells();
	virtual Hit calculateIntersection();

private:
	//std::vector<SceneObject*>* Objects;
	std::vector<SceneObject*> Cells; //cells are stored in a 1D array
	BBox Bbox;	
	int Nx, Ny, Nz; //number of cells in the x, y, z directions;

	Vector3 getMinCoords(); //compute minimum grid coordinates
	Vector3 getMaxCoords();	// compute maximum grid coordinates
};

