#pragma once
#include <iostream>
#include "Scene.h"

class OBJ
{
public:
	OBJ(Scene* s) : scene(s) {}
	~OBJ() = default;
	Scene* scene;
	void parseLine(std::stringstream& in, std::ifstream& file);

private:

	
};
