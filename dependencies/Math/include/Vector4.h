#pragma once
#ifndef VECTOR4_H
#define VECTOR4_H
#include <iostream>
#include "Vector3.h"

class Vector4
{
public:
	float coordinates[4];
	Vector4();
	Vector4(float x, float y, float z, float w);
	Vector4(Vector3 v);
	Vector4 operator+(Vector4 vector);
	Vector4 operator-(Vector4 vector);
	Vector4 operator*(Vector4 vector);
	Vector4 operator*(float scalar);
	Vector4 clear();
	Vector4 operator/(float scalar);
	Vector4 operator=(Vector4 vector);
	bool operator==(Vector4 vector);
	bool operator!=(Vector4 vector);
	Vector4 normalize();
	float magnitude();
	float quadrance();
	friend std::ostream& operator<<(std::ostream&, Vector4 vec);
	~Vector4();
};	
#endif
