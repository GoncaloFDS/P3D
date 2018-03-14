#include "Vector3.h"
#ifndef THRESHOLD 
#define THRESHOLD 0.0005
#endif


Vector3::Vector3()
{
	coordinates[0] = 0;
	coordinates[1] = 0;
	coordinates[2] = 0;
}

Vector3::Vector3(float x, float y, float z)
{
	coordinates[0] = x;
	coordinates[1] = y;
	coordinates[2] = z;
}

Vector3 Vector3::operator-() {
	return Vector3(-coordinates[0], -coordinates[1], -coordinates[2]);
}
Vector3::~Vector3()
{
}

Vector3 Vector3::operator+(const Vector3 vector)
{
	return Vector3(coordinates[0] + vector.coordinates[0], coordinates[1] + vector.coordinates[1], coordinates[2] + vector.coordinates[2]);
}

Vector3 Vector3::operator+=(const Vector3 vector)
{
	this->coordinates[0] += vector.coordinates[0];
	this->coordinates[1] += vector.coordinates[1];
	this->coordinates[2] += vector.coordinates[2];
	return *this;
}

Vector3 Vector3::operator-=(const Vector3 vector)
{
	coordinates[0] -= vector.coordinates[0];
	coordinates[1] -= vector.coordinates[1];
	coordinates[2] -= vector.coordinates[2];
	return *this;
}

Vector3 Vector3::operator-(const Vector3 vector)
{
	return Vector3(coordinates[0] - vector.coordinates[0], coordinates[1] - vector.coordinates[1], coordinates[2] - vector.coordinates[2]);
}

float Vector3::operator*(const Vector3 vector)
{
	return (coordinates[0] * vector.coordinates[0]) + (coordinates[1] * vector.coordinates[1]) + (coordinates[2] * vector.coordinates[2]);
}

Vector3 Vector3::operator*(const float c)
{
	return Vector3(c*coordinates[0], c*coordinates[1], c*coordinates[2]);
}

Vector3 Vector3::operator/(const float c)
{
	return operator*((1 / c));
}

Vector3 Vector3::operator=(const Vector3 vector)
{
	coordinates[0] = vector.coordinates[0];
	coordinates[1] = vector.coordinates[1];
	coordinates[2] = vector.coordinates[2];

	return *this;
}

Vector3 Vector3::crossProduct(Vector3 u, Vector3 v)
{
	
	float x = (u.coordinates[1] * v.coordinates[2]) - (v.coordinates[1] * u.coordinates[2]);
	float y = (u.coordinates[2] * v.coordinates[0]) - (v.coordinates[2] * u.coordinates[0]);
	float z = (u.coordinates[0] * v.coordinates[1]) - (v.coordinates[0] * u.coordinates[1]);
	return Vector3(x,y,z);
}

bool Vector3::operator==(const Vector3 vector)
{
	return (fabs(coordinates[0] - vector.coordinates[0]) < THRESHOLD) && (fabs(coordinates[1] - vector.coordinates[1]) < THRESHOLD) && (fabs(coordinates[2] - vector.coordinates[2]) < THRESHOLD);
}

bool Vector3::operator!=(const Vector3 vector)
{
	return !operator==(vector);
}

Vector3 operator*(float c, Vector3 vector)
{
	return vector*c;
}

std::ostream & operator<<(std::ostream &os, Vector3 vector)
{
	os << "Vector3[" << vector.coordinates[0] << "," << vector.coordinates[1] << "," << vector.coordinates[2] << "]";
	return os;
}

std::istream & operator>>(std::istream &in, Vector3& v)
{
	in >> v.coordinates[0];
	in >> v.coordinates[1];
	in >> v.coordinates[2];
	return in;
}




float Vector3::quadrance()
{
	return (coordinates[0] * coordinates[0]) + (coordinates[1] * coordinates[1]) + (coordinates[2] * coordinates[2]);
}

float Vector3::magnitude()
{
	return sqrt(quadrance());
}

Vector3 Vector3::normalize()
{
	float abs = magnitude();
	Vector3 normalized = Vector3(0, 0, 0);
	if (abs > 0) {
		normalized.coordinates[0] = (1 / abs) * coordinates[0];
		normalized.coordinates[1] = (1 / abs) * coordinates[1];
		normalized.coordinates[2] = (1 / abs) * coordinates[2];
	}
	else {
			throw std::exception("Invalid operation. Can't normalize null vector.");
	}
	return normalized;
}

Vector3 Vector3::clear()
{
	for (auto i = 0; i < 2; i++)
		if (fabs(this->coordinates[0]) < THRESHOLD)
			this->coordinates[i] = 0;
	return *this;
}




