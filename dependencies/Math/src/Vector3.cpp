#include "Vector3.h"
#ifndef THRESHOLD 
#define THRESHOLD 0.0005
#endif


Vector3::Vector3()
{
	xyz[0] = 0;
	xyz[1] = 0;
	xyz[2] = 0;
}

Vector3::Vector3(float x, float y, float z)
{
	xyz[0] = x;
	xyz[1] = y;
	xyz[2] = z;
}

Vector3::Vector3(float f)
{
	xyz[0] = f;
	xyz[1] = f;
	xyz[2] = f;

}

Vector3 Vector3::operator-() {
	return Vector3(-xyz[0], -xyz[1], -xyz[2]);
}
Vector3::~Vector3()
{
}

Vector3 Vector3::operator+(const Vector3 vector)
{
	return Vector3(xyz[0] + vector.xyz[0], xyz[1] + vector.xyz[1], xyz[2] + vector.xyz[2]);
}

Vector3 Vector3::operator+=(const Vector3 vector)
{
	this->xyz[0] += vector.xyz[0];
	this->xyz[1] += vector.xyz[1];
	this->xyz[2] += vector.xyz[2];
	return *this;
}

Vector3 Vector3::operator-=(const Vector3 vector)
{
	xyz[0] -= vector.xyz[0];
	xyz[1] -= vector.xyz[1];
	xyz[2] -= vector.xyz[2];
	return *this;
}

Vector3 Vector3::operator-(const Vector3 vector)
{
	return Vector3(xyz[0] - vector.xyz[0], xyz[1] - vector.xyz[1], xyz[2] - vector.xyz[2]);
}

float Vector3::operator*(const Vector3 vector)
{
	return (xyz[0] * vector.xyz[0]) + (xyz[1] * vector.xyz[1]) + (xyz[2] * vector.xyz[2]);
}

Vector3 Vector3::operator*(const float c)
{
	return Vector3(c*xyz[0], c*xyz[1], c*xyz[2]);
}

Vector3 Vector3::operator/(const float c)
{
	return operator*((1 / c));
}

Vector3 Vector3::operator=(const Vector3 vector)
{
	xyz[0] = vector.xyz[0];
	xyz[1] = vector.xyz[1];
	xyz[2] = vector.xyz[2];

	return *this;
}

Vector3 Vector3::crossProduct(Vector3 u, Vector3 v)
{
	
	float x = (u.xyz[1] * v.xyz[2]) - (v.xyz[1] * u.xyz[2]);
	float y = (u.xyz[2] * v.xyz[0]) - (v.xyz[2] * u.xyz[0]);
	float z = (u.xyz[0] * v.xyz[1]) - (v.xyz[0] * u.xyz[1]);
	return Vector3(x,y,z);
}

Vector3 Vector3::cross(Vector3 other)
{
	return crossProduct(*this, other);
}

bool Vector3::operator==(const Vector3 vector)
{
	return (fabs(xyz[0] - vector.xyz[0]) < THRESHOLD) && (fabs(xyz[1] - vector.xyz[1]) < THRESHOLD) && (fabs(xyz[2] - vector.xyz[2]) < THRESHOLD);
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
	os << "Vector3[" << vector.xyz[0] << "," << vector.xyz[1] << "," << vector.xyz[2] << "]";
	return os;
}

std::istream & operator>>(std::istream &in, Vector3& v)
{
	in >> v.xyz[0];
	in >> v.xyz[1];
	in >> v.xyz[2];
	return in;
}




float Vector3::quadrance()
{
	return (xyz[0] * xyz[0]) + (xyz[1] * xyz[1]) + (xyz[2] * xyz[2]);
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
		normalized.xyz[0] = (1 / abs) * xyz[0];
		normalized.xyz[1] = (1 / abs) * xyz[1];
		normalized.xyz[2] = (1 / abs) * xyz[2];
	}
	else {
			throw std::exception("Invalid operation. Can't normalize null vector.");
	}
	return normalized;
}

Vector3 Vector3::clear()
{
	for (auto i = 0; i < 2; i++)
		if (fabs(this->xyz[0]) < THRESHOLD)
			this->xyz[i] = 0;
	return *this;
}




