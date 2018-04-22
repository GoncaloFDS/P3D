#pragma once
#include <string>
#include <sstream>
#include <istream>
#include <iostream>
#include <fstream>
#include <vector>
#include "Camera.h"
#include "Vector3.h"
#include "Light.h"
#include "Material.h"
#include "SceneObject.h"
#include "Sphere.h"
#include "AreaLight.h"

class Scene
{
public:
	Scene();
	~Scene();
	std::vector<SceneObject*> getObjects();
	bool loadNFF(std::string filepath);
	Camera* getCamera();
	std::vector<Light*> getLights();
	Vector3 backgroundColor;

	void generateBB();

private:
	Camera *camera;
	std::vector<Light*> LightVector;
	AreaLight *areaLight;
	std::vector<SceneObject*> objects;
	std::vector<Material> materials;
	std::ifstream fileScene;

	void parseBackgroundColor( std::stringstream& in);
	void parseFrom(std::stringstream& in);
	void parseAt(std::stringstream& in);
	void parseUp(std::stringstream& in);
	void parseAngle(std::stringstream& in);
	void parseHither(std::stringstream& in);
	void parseResolution(std::stringstream& in);
	void parseLight( std::stringstream& in);
	void parseObjectMaterials( std::stringstream& in);
	void parseCone(std::stringstream& in);
	void parseSphere(std::stringstream& in);
	void parsePlane(std::stringstream& in);
	void parsePolygon(std::stringstream& in, std::ifstream& file);
	void parseLine(std::stringstream& in, std::ifstream& file);
	void parseAreaLight(std::stringstream& in);
	void parseLensRadius(std::stringstream& in);
	void parseApperture(std::stringstream & in);
	void parseFocalPlane(std::stringstream & in);
	void parseViewPlane(std::stringstream & in);

};

