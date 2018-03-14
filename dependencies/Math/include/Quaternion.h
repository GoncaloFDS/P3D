#pragma once

#ifndef QUATERNION_H
#define QUATERNION_H
#include "Vector4.h"
#include "Matrix4.h"
#define THRESHOLD 0.0005

class Matrix4;

class Quaternion
{
public:
	float t, x, y, z;
	Quaternion();
	~Quaternion();
	Quaternion(float t, float x, float y, float z);
	Quaternion(float radianAngle, Vector4 axis);
	static Quaternion FromAngleAxis(const float thetaAngle, Vector4 axis);
	const void QuaternionToAngleAxis(float& theta, Vector4& axis);
	const float QuaternionQuadrance();
	const float QuaternionNorm();
	Quaternion NormalizeQuaternion();
	const Quaternion Conjugate();
	const Quaternion InverseQuaternion();
	Quaternion operator+(Quaternion q1);
	Quaternion operator*(Quaternion q1);
	Quaternion operator*(const float factor);
	Quaternion operator=(Quaternion q1);
	Matrix4 QuaternionToGLMatrix();
	static Quaternion Lerp(Quaternion q0, Quaternion q1, float k);
	static Quaternion Slerp(Quaternion q0, Quaternion q1, float k);
	bool operator==(Quaternion q1);
	bool operator!=(Quaternion q2);
	Quaternion Clean();
	void PrintAngleAxis(std::string);
	friend std::ostream& operator<<(std::ostream& os, Quaternion q1);
	Matrix4 ToGLMatrix();

};




#endif