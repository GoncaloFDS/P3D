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
#include "Grid.h"

class Scene {
public:
	Scene();
	~Scene() = default;
	bool loadNFF(std::string filepath);
	Camera* getCamera();
	std::vector<SceneObject*> *getObjects();
	std::vector<Light*> *getLights();
	std::vector<Material*> *getMaterials();
	Vector3 backgroundColor;
	void setupGrid();
	bool validGrid() { return grid != nullptr; }
	void enableGrid() { gridEnabled = true; }
	void disableGrid() { gridEnabled = false; }
	Hit calculateClossestHit(Ray& ray);

private:
	Camera *camera;
	AreaLight *areaLight;
	std::vector<Light*> *LightVector;
	std::vector<SceneObject*> *objects;
	std::vector<Material*> *materials;
	std::ifstream fileScene;
	Grid *grid = nullptr;
	bool gridEnabled = false;
};

