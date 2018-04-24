#include "NFF.h"
#include "ThinLens.h"
#include "Plane.h"
#include "Triangle.h"

void NFF::parseBackgroundColor(std::stringstream& in)
{
	Vector3 v;
	in >> v;
	scene->backgroundColor = v;
	std::cout << "BackgroundColor: " << v << std::endl;
}

void NFF::parseFrom(std::stringstream& in)
{
	in >> scene->getCamera()->Eye;
	std::cout << "From: " << scene->getCamera()->Eye << std::endl;
}
void NFF::parseAt(std::stringstream& in)
{
	in >> scene->getCamera()->At;
	std::cout << "At: " << scene->getCamera()->At << std::endl;
}
void NFF::parseUp(std::stringstream& in)
{
	in >> scene->getCamera()->Up;
	std::cout << "Up: " << scene->getCamera()->Up << std::endl;
}
void NFF::parseAngle(std::stringstream& in)
{
	in >> scene->getCamera()->Fovy;
	std::cout << "Angle: " << scene->getCamera()->Fovy << std::endl;
}
void NFF::parseHither(std::stringstream& in)
{
	in >> scene->getCamera()->Near;
	if (!static_cast<ThinLens*>(scene->getCamera())) {
		static_cast<ThinLens*>(scene->getCamera())->d = scene->getCamera()->Near;
		static_cast<ThinLens*>(scene->getCamera())->f = (scene->getCamera()->Eye - scene->getCamera()->At).magnitude();

	}
	std::cout << "Hither: " << scene->getCamera()->Near << std::endl;
	scene->getCamera()->Far = 1000 * scene->getCamera()->Near;


}
void NFF::parseResolution(std::stringstream& in)
{
	in >> scene->getCamera()->Resolution;
	std::cout << "Res: " << scene->getCamera()->Resolution << std::endl;
}

void NFF::parseLight(std::stringstream& in) {
	Vector3 pos, color;
	in >> pos;
	in >> color;
	auto *light = new Light(pos, color);
	scene->getLights()->push_back(light);
	std::cout << "Light position: " << light->getPoint() << std::endl;
}

void NFF::parseObjectMaterials(std::stringstream& in)
{
	Vector3 color;
	Material *material = new Material();;
	in >> color;
	material->color = color;
	in >> material->Kd;
	in >> material->Ks;
	in >> material->shininess;
	in >> material->T;
	if (material->T > 0)
		material->isTranslucid = true;
	in >> material->refractionIndex;
	in >> material->glosiness;
	std::cout << "Refraction index: " << material->refractionIndex << std::endl;
	std::cout << "Material transmittance: " << material->T << std::endl;
	if(material->glosiness > 0)
		std::cout << "Material glosiness: " << material->glosiness << std::endl;
	scene->getMaterials()->push_back(material);
}

void NFF::parseSphere(std::stringstream& in)
{
	auto sphere = new Sphere();
	in >> sphere->center;
	in >> sphere->radius;
	sphere->material = scene->getMaterials()->back();
	scene->getObjects()->push_back(sphere);
	std::cout << "Sphere: Center->" << sphere->center << " Radius: " << sphere->radius << std::endl;
}

void NFF::parsePlane(std::stringstream& in) {
	Vector3 p1, p2, p3;
	in >> p1 >> p2 >> p3;
	Plane *plane = new Plane(p1, p2, p3);
	plane->material = scene->getMaterials()->back();
	scene->getObjects()->push_back(plane);
	std::cout << "Plane: p1->" << plane->P1 << "Plane: p2->" << plane->P2 << "Plane: p3->" << plane->P3 << std::endl;
}

void NFF::parsePolygon(std::stringstream& in, std::ifstream& file) {
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
	triangle->material = scene->getMaterials()->back();
	scene->getObjects()->push_back(triangle);
}

void NFF::parseLine(std::stringstream& in, std::ifstream& file)
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
	else if (s == "obj") //.obj file name
		parseOBJ(in);
}

void NFF::parseAreaLight(std::stringstream& in) {
	Vector3 pos, va, vb, color;
	int sampleSize;
	in >> pos >> va >> vb >> color >> sampleSize;
	auto areaLight = new AreaLight(pos, va, vb, color, sampleSize);
	scene->getLights()->push_back(areaLight);
}

void NFF::parseLensRadius(std::stringstream & in)
{
	in >> (static_cast<ThinLens*>(scene->getCamera()))->radius;
}

void NFF::parseApperture(std::stringstream & in)
{
	in >> (static_cast<ThinLens*>(scene->getCamera()))->apperture;
}

void NFF::parseFocalPlane(std::stringstream & in)
{
	in >> (static_cast<ThinLens*>(scene->getCamera()))->f;
}

void NFF::parseViewPlane(std::stringstream & in)
{
	in >> (static_cast<ThinLens*>(scene->getCamera()))->d;
}

void NFF::parseOBJ(std::stringstream & in)
{

}
