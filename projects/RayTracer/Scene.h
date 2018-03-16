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

class Scene
{
public:
	Scene();
	~Scene();
	std::vector<SceneObject*> GetObjects();
	bool load_nff(std::string filepath);
	Vector3 backgroundColor;
	Camera* GetCamera();
	std::vector<PointLight*> getLights();
private:
	Camera *camera;
	std::vector<PointLight*> lightPoints;
	std::vector<SceneObject*> objects;
	std::vector<Material> materials;
	void parseBackgroundColor( std::stringstream& in);
	void parseFrom(std::stringstream& in);
	void parseAt(std::stringstream& in);
	void parseUp(std::stringstream& in);
	void parseAngle(std::stringstream& in);
	void parseHither(std::stringstream& in);
	void parseResolution(std::stringstream& in);
	void parseViewingAngles( std::stringstream& in);
	void parseLightPosition( std::stringstream& in);
	void parseObjectMaterials( std::stringstream& in);
	void parseCone( std::stringstream& in);
	void parseSphere( std::stringstream& in);
	void parsePlane( std::stringstream& in);
	void parsePolygonPatch( std::stringstream& in);
	void ParseLine(std::stringstream& in);

};

