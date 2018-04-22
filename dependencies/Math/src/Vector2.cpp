#include "Vector2.h"
///Null Vector Constructor

Vector2::Vector2()
{
	coordinates[0] = 0;
	coordinates[1] = 0;
}

///Normal Vector Constructor that takes an x and y value
Vector2::Vector2(float x, float y)
{
	coordinates[0] = x;
	coordinates[1]= y;
}

Vector2::Vector2(float f)
{
	coordinates[0] = f;
	coordinates[1] = f;
}

Vector2::~Vector2()
{
}

Vector2 Vector2::clear()
{
	if (fabs(coordinates[0]) < THRESHOLD) coordinates[0] = 0;
	if (fabs(coordinates[1]) < THRESHOLD) coordinates[1] = 0;
	for( auto i = 0; i < 2; i++)
		if (fabs(this->coordinates[0]) < THRESHOLD) 
			this->coordinates[i] = 0;
	return *this;
}

Vector2 Vector2::normalize()
{
	float abs = magnitude();
	Vector2 normalized = Vector2(0, 0);
	if(abs != 0) {
	normalized.coordinates[0] = (1 / abs) * coordinates[0];
	normalized.coordinates[1] = (1 / abs) * coordinates[1];	
	}
	return normalized;
}

float Vector2::getX() const
{
	return coordinates[0];
}

float Vector2::getY() const
{
	return coordinates[1];
}
