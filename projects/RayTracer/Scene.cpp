#include "Scene.h"
#include "../../Material.h"
#include "../../Sphere.h"
#include "../../Plane.h"


Scene::Scene()
{
	camera = new Camera();
}


Scene::~Scene()
{
}

std::vector<SceneObject*> Scene::GetObjects()
{
	return objects;
}

bool Scene::load_nff(std::string filepath)
{
	std::ifstream file = std::ifstream(filepath);
	if (file.fail()) {
		std::string error = "The nff file " + filepath + " seems to be missing.";
		return false;
		throw std::ios_base::failure(error);
	}
	std::string line;
	while (std::getline(file, line)) {
		ParseLine(std::stringstream(line));
	}
	file.close();
	return true;

}

Camera* Scene::GetCamera()
{
	return camera;
}

void Scene::parseBackgroundColor(std::stringstream& in)
{
	Vector3 v;
	in >> v;
	backgroundColor = v;
	std::cout << "BackgroundColor: " << v << std::endl;
}

void Scene::parseFrom(std::stringstream& in)
{
	in >> camera->Eye;
	std::cout << "From: " << camera->Eye << std::endl;
}
void Scene::parseAt(std::stringstream& in)
{
	in >> camera->At;
	std::cout << "At: " << camera->At << std::endl;
}
void Scene::parseUp(std::stringstream& in)
{
	in >> camera->Up;
	std::cout << "Up: " << camera->Up << std::endl;
}
void Scene::parseAngle(std::stringstream& in)
{
	in >> camera->Fovy;
	std::cout << "Angle: " << camera->Fovy << std::endl;
}
void Scene::parseHither(std::stringstream& in)
{
	in >> camera->Near;
	std::cout << "Hither: " << camera->Near << std::endl;
	camera->Far = 1000 * camera->Near;
}
void Scene::parseResolution(std::stringstream& in)
{
	in >> camera->Resolution;
	std::cout << "Res: " << camera->Resolution << std::endl;
}

void Scene::parseLightPosition(std::stringstream& in)
{
	auto *light = new PositionalLight();
	in >> light->Position;
	Vector3 color;
	in >> color;
	light->Color = color;
	lightPoints.push_back(light);
	std::cout << "Light position: " << light->Position << std::endl;
}

void Scene::parseObjectMaterials(std::stringstream& in)
{
	Vector3 color;
	Material material;
	in >> color;
	material.color = color;
	in >> material.Kd;
	in >> material.Ks;
	in >> material.shininess;
	in >> material.T;
	in >> material.refractionIndex;	
	materials.push_back(material);
}

void Scene::parseCone( std::stringstream& in)
{
}

void Scene::parseSphere( std::stringstream& in)
{
	Sphere* sphe = new Sphere();
	in >> sphe->center;
	in >> sphe->radius;
	sphe->material = materials.back();
	objects.push_back(sphe);
	std::cout << "Sphere: Center->" << sphe->center << " Radius: " << sphe->radius << std::endl;
}

void Scene::parsePlane( std::stringstream& in) {
	Vector3 p1, p2, p3;
	in >> p1 >> p2 >> p3;
	Plane *plane = new Plane(p1, p2, p3);
	plane->material = materials.back();
	objects.push_back(plane);
	std::cout << "Plane: p1->" << plane->P1 << "Plane: p2->" << plane->P2 << "Plane: p3->" << plane->P3 <<std::endl;
}

void Scene::parsePolygonPatch( std::stringstream& in)
{
}

void Scene::ParseLine(std::stringstream& in)
{
		std::string s;
		in >> s;
		if (s == "b") //backgroundColor
			parseBackgroundColor(in);
		else if (s == "from")
			parseFrom(in);
		else if (s == "at")
			parseAt(in);
		else if (s == "up")
			parseUp(in);
		else if (s == "angle")
			parseAngle(in);
		else if (s == "hither")
			parseHither(in);
		else if (s == "resolution")
			parseResolution(in);
		else if (s == "l") //LightPosition
			parseLightPosition(in);
		else if (s == "f")
			parseObjectMaterials(in); //Object Materials
		else if (s == "c") //cone primitive
			parseCone(in);
		else if (s == "s")
			parseSphere(in); // sphere primitive
		else if (s == "pl")
			parsePlane(in); // Polygon primitive
		else if (s == "pp")
			parsePolygonPatch(in); // Polygon Patch
}

std::vector<PositionalLight*> Scene::getLights()
{
	return lightPoints;
}
