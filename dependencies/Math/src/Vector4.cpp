#include "Vector4.h"
#define THRESHOLD 0.0005


Vector4::Vector4()
{
}

Vector4::Vector4(float x, float y, float z, float w)
{
	coordinates[0] = x;
	coordinates[1] = y;
	coordinates[2] = z;
	coordinates[3] = w;
}

Vector4::Vector4(Vector3 v)
{
	coordinates[0] = v.coordinates[0];
	coordinates[1] = v.coordinates[1];
	coordinates[2] = v.coordinates[2];
	coordinates[3] = 1;
}

Vector4 Vector4::operator+(Vector4 vector)
{
	Vector4 sum;
	for(int i = 0; i < 4; i++)
	{
		sum.coordinates[i] = this->coordinates[i] + vector.coordinates[i];
	}
	return sum;
}


Vector4 Vector4::operator=(Vector4 vector)
{
	this->coordinates[0] = vector.coordinates[0];
	this->coordinates[1] = vector.coordinates[1];
	this->coordinates[2] = vector.coordinates[2];
	this->coordinates[3] = vector.coordinates[3];

	return *this;
}

bool Vector4::operator==(Vector4 vector)
{
	return (fabs(coordinates[0] - vector.coordinates[0]) < THRESHOLD && fabs(coordinates[1] - vector.coordinates[1]) < THRESHOLD && 
		fabs(coordinates[2] - vector.coordinates[2]) < THRESHOLD && fabs(coordinates[3] - vector.coordinates[3]) < THRESHOLD);
}

bool Vector4::operator!=(Vector4 vector)
{
	return !operator==(vector);
}

Vector4 Vector4::normalize()
{
	float abs = magnitude();
	Vector4 normalized = Vector4(0, 0, 0, 0);
	if (abs > 0) {
		normalized.coordinates[0] = (1 / abs) * coordinates[0];
		normalized.coordinates[1] = (1 / abs) * coordinates[1];
		normalized.coordinates[2] = (1 / abs) * coordinates[2];
		normalized.coordinates[3] = 1;
	}
	else {
		throw std::exception("Invalid operation. Can't normalize null vector.");
	}
	return normalized;
}

float Vector4::magnitude()
{
	return this->coordinates[3]*sqrt(quadrance());
}

float Vector4::quadrance()
{
	return this->coordinates[0] * this->coordinates[0] + this->coordinates[1] * this->coordinates[1] + this->coordinates[2] * this->coordinates[2];
}

Vector4::~Vector4()
{
}

std::ostream & operator<<(std::ostream & os, Vector4 vec)
{
	os << "Vector4[" << vec.coordinates[0] << "," << vec.coordinates[1] << "," << vec.coordinates[2] << "," << vec.coordinates[3] << "]";

	return os;
}
