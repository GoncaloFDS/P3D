#pragma once
#include <string>
#include "Color.h"
#include "Vector3.h"
#include <sstream>
#include <istream>
#include <iostream>
#include <fstream>
#include "Camera.h"
#include <vector>
#include "../../PointLight.h"
#include "../../Material.h"
#include "../../SceneObject.h"
#include "../../Sphere.h"
#include "../../AreaLight.h"

class Scene
{
public:
	Scene();
	~Scene();
	std::vector<SceneObject*> getObjects();
	bool loadNFF(std::string filepath);
	Camera* getCamera();
	std::vector<PointLight*> getLights();
	Vector3 backgroundColor;

private:
	Camera *camera;
	std::vector<PointLight*> lightPoints;
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
	void parseLightPosition( std::stringstream& in);
	void parseObjectMaterials( std::stringstream& in);
	void parseCone(std::stringstream& in);
	void parseSphere(std::stringstream& in);
	void parsePlane(std::stringstream& in);
	void parsePolygon(std::stringstream& in, std::ifstream& file);
	void ParseLine(std::stringstream& in, std::ifstream& file);
	void ParseAreaLight(std::stringstream& in);

};

