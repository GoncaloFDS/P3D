#include "Color.h"
#include "Vector3.h"


Color::Color()
{
}


Color::~Color()
{
}

Color::Color(float _r, float _g, float _b)
{
	this->r = _r;
	this->g = _g;
	this->b = _b;
}

Color::Color(Vector3 vec3)
{
	this->r = vec3.coordinates[0];
	this->g = vec3.coordinates[1];
	this->b = vec3.coordinates[2];
}