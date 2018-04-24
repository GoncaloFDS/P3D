#pragma once
#include <iostream>
#include "Scene.h"

class NFF
{
public:
	NFF(Scene* s) : scene(s) {}
	~NFF() = default;
	Scene* scene;
	void parseLine(std::stringstream& in, std::ifstream& file);

private:

	void parseBackgroundColor(std::stringstream& in);
	void parseFrom(std::stringstream& in);
	void parseAt(std::stringstream& in);
	void parseUp(std::stringstream& in);
	void parseAngle(std::stringstream& in);
	void parseHither(std::stringstream& in);
	void parseResolution(std::stringstream& in);
	void parseLight(std::stringstream& in);
	void parseObjectMaterials(std::stringstream& in);
	void parseSphere(std::stringstream& in);
	void parsePlane(std::stringstream& in);
	void parsePolygon(std::stringstream& in, std::ifstream& file);
	void parseAreaLight(std::stringstream& in);
	void parseLensRadius(std::stringstream& in);
	void parseApperture(std::stringstream & in);
	void parseFocalPlane(std::stringstream & in);
	void parseViewPlane(std::stringstream & in);
	void parseOBJ(std::stringstream & in);
};

