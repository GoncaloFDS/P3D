#include "Scene.h"
#include "Material.h"
#include "Sphere.h"
#include "Plane.h"
#include "Triangle.h"
#include "AreaLight.h"
#include "Light.h"
#include "ThinLens.h"

Scene::Scene()
{
	camera = new ThinLens();
}


Scene::~Scene()
{
}

std::vector<SceneObject*> Scene::getObjects()
{
	return objects;
}

bool Scene::loadNFF(std::string filepath)
{	
	std::ifstream file = std::ifstream(filepath);
	if (file.fail()) {
		std::string error = "The nff file " + filepath + " seems to be missing.";
		return false;
		throw std::ios_base::failure(error);
	}
	std::string line;
	while (std::getline(file, line)) {
		parseLine(std::stringstream(line), file);
	}
	file.close();
	return true;

}

Camera* Scene::getCamera()
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
	if (!static_cast<ThinLens*>(camera)) {
		static_cast<ThinLens*>(camera)->d = camera->Near;
		static_cast<ThinLens*>(camera)->f = (camera->Eye - camera->At).magnitude();

	}
	std::cout << "Hither: " << camera->Near << std::endl;
	camera->Far = 1000 * camera->Near;
	

}
void Scene::parseResolution(std::stringstream& in)
{
	in >> camera->Resolution;
	std::cout << "Res: " << camera->Resolution << std::endl;
}

void Scene::parseLight(std::stringstream& in) {
	Vector3 pos, color;
	in >> pos;
	in >> color;
	auto *light = new Light(pos, color);
	LightVector.push_back(light);
	std::cout << "Light position: " << light->getPoint() << std::endl;
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
	if (material.T > 0)
		material.isTranslucid = true;
	in >> material.refractionIndex;	
	std::cout << "Refraction index: " << material.refractionIndex << std::endl;
	std::cout << "Material transmittance: " << material.T << std::endl;
	materials.push_back(material);
}

void Scene::parseCone( std::stringstream& in)
{
}

void Scene::parseSphere( std::stringstream& in)
{
	auto sphe = new Sphere();
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

void Scene::parsePolygon(std::stringstream& in, std::ifstream& file) {
	int n;
	Vector3 verts[3];
	in >> n;
	if (n != 3) 
		throw std::runtime_error("Parsing polygons that aren't triangles is not supported");
	
	std::string line;
	for (int i = 0; i < n; i++) {
		std::getline(file, line);
		auto newLine = std::stringstream(line);
		newLine >> verts[i];
	}
	
	Triangle* triangle = new Triangle(verts[0], verts[1], verts[2]);
	triangle->material = materials.back();
	objects.push_back(triangle);
}

void Scene::parseLine(std::stringstream& in, std::ifstream& file)
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
		parseLight(in);
	else if (s == "f")
		parseObjectMaterials(in); //Object Materials
	else if (s == "c") //cone primitive
		parseCone(in);
	else if (s == "s")
		parseSphere(in); // sphere primitive
	else if (s == "pl")
		parsePlane(in); // infinite plane
	else if (s == "p")
		parsePolygon(in, file); // polygon
	else if (s == "al")
		parseAreaLight(in);
	else if (s == "lensRadius")
		parseLensRadius(in);
	else if (s == "apperture")
		parseApperture(in);
	else if (s == "focalp")
		parseFocalPlane(in);
	else if (s == "viewp")
		parseViewPlane(in);

}

void Scene::parseAreaLight(std::stringstream& in) {
	Vector3 pos, va, vb, color;
	in >> pos >> va >> vb >> color;
	areaLight = new AreaLight(pos, va, vb, color, 10);
	LightVector.push_back(areaLight);
	//std::cout << "Area Light c: " << areaLight->c << " a: " << areaLight->a << " b: " << areaLight->b;
}

void Scene::parseLensRadius(std::stringstream & in)
{
	float radius;
	in >> (static_cast<ThinLens*>(camera))->radius;
}

void Scene::parseApperture(std::stringstream & in)
{
	float apperture;
	in >> (static_cast<ThinLens*>(camera))->apperture;
}

void Scene::parseFocalPlane(std::stringstream & in)
{
	float f;
	in >> (static_cast<ThinLens*>(camera))->f;
}

void Scene::parseViewPlane(std::stringstream & in)
{
	float d;
	in >> (static_cast<ThinLens*>(camera))->d;
}

std::vector<Light*> Scene::getLights()
{
	return LightVector;
}

void Scene::generateBB()
{

}
