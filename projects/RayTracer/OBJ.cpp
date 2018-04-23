#include "OBJ.h"
#include "Triangle.h"
#include <string>

void OBJ::parseLine(std::stringstream& in, std::ifstream& file)
{

}


void OBJ::parseVertexs(std::stringstream & in)
{
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

void OBJ::parseTextures(std::stringstream & in)
{
}

void OBJ::parseNormals(std::stringstream & in)
{
	}

void OBJ::parseFaces(std::stringstream & in)
{
	std::string token;
	for (int i = 0; i < 3; i++) {
		std::getline(in, token, '/');
		if (token.size() > 0)
			this->VerticesIdx.push_back(std::stoi(token));
		std::getline(in, token, '/');
		if (token.size() > 0)
			this->TexsIdx.push_back(std::stoi(token));
		std::getline(in, token, ' ');
		if (token.size() > 0)
			this->NormalsIdx.push_back(std::stoi(token));
	}
	//std::cout << in.str() << std::endl;
}

void OBJ::parseComments(std::stringstream & in)
{
	//std::cout << in.str() << std::endl;
}