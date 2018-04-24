#include "Scene.h"
#include "Material.h"
#include "Sphere.h"
#include "Plane.h"
#include "Triangle.h"
#include "AreaLight.h"
#include "Light.h"
#include "ThinLens.h"
#include "Grid.h"
#include "NFF.h"

Scene::Scene()
{
	camera = new ThinLens();
	objects = new std::vector<SceneObject *>;
	materials = new std::vector<Material *>;
	LightVector = new std::vector<Light *>;
}

std::vector<SceneObject*> *Scene::getObjects()
{
	return objects;
}

bool Scene::loadNFF(std::string filepath)
{	
	NFF nff(this);
	std::ifstream file = std::ifstream(filepath);
	if (file.fail()) {
		std::string error = "The nff file " + filepath + " seems to be missing.";
		return false;
		throw std::ios_base::failure(error);
	}
	std::string line;
	while (std::getline(file, line)) {
		nff.parseLine(std::stringstream(line), file);
	}
	file.close();
	return true;

}

Camera* Scene::getCamera()
{
	return camera;
}



std::vector<Light*> *Scene::getLights()
{
	return LightVector;
}

std::vector<Material*> *Scene::getMaterials()
{
	return materials;
}

void Scene::setupGrid() {
	grid = new Grid(objects);
	grid->setupCells();
	gridEnabled = true;
}

Hit Scene::calculateClossestHit(Ray& ray)
{	
	if(validGrid() && gridEnabled)
		return grid->hit(ray);
	else {
		double Tmin = DBL_MAX;
		Hit hit;
		for (auto obj : *objects) {
			Hit tempHit = obj->calculateIntersection(ray);
			if ((tempHit.HasCollided && tempHit.T < Tmin)) {
				Tmin = tempHit.T;
				hit = tempHit;
				hit.Mat = obj->material;
			}
		}	return hit;
	}
}

