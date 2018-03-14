#pragma once
#ifndef VECTOR3_H
#define VECTOR3_H
#include <iostream>
#include <sstream>
#include <string>

class Vector3
{
public:
	float coordinates[3];
	Vector3();
	Vector3(float, float, float);
	Vector3 operator-();
	~Vector3();
	Vector3 operator+(const Vector3 vector);
	Vector3 operator+=(const Vector3 vector);
	Vector3 operator-=(const Vector3 vector);
	Vector3 operator-(const Vector3 vector);
	float operator*(const Vector3 vector);
	Vector3 operator*(const float c);
	friend Vector3 operator*(float, Vector3);
	Vector3 operator/(const float vector);
	Vector3 operator=(const Vector3 vector);
	static Vector3 crossProduct(Vector3, Vector3);
	bool operator==(const Vector3 vector);
	bool operator!=(const Vector3 vector);
	friend std::ostream& operator<<(std::ostream&, Vector3);
	friend std::istream& operator>>(std::istream&, Vector3&);
	float quadrance();
	float magnitude();
	Vector3 normalize();
	Vector3 clear();

};

#endif
