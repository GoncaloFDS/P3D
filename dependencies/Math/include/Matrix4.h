#pragma once
#ifndef MATRIX4_H
#define MATRIX4_H
#define THRESHOLD 0.0005
#include "Vector4.h"
#include <iostream>

class Matrix3;
enum AXIS;
class Vector3;
class Matrix4
{
public:
	float data[16];
	Matrix4();
	Matrix4(float x1, float y1, float z1, float w1, float x2, float y2, float z2, float w2, float x3, float y3, float z3, float w3, float x4, float y4, float z4, float w4);
	const static Matrix4 ZERO;
	const static Matrix4 IDENTITY;
	static Matrix4 ROTATE(AXIS axis, float angle);
	static Matrix4 SCALE(float x, float y, float z);
	static Matrix4 TRANSLATE(float x, float y, float z);
	Matrix4 operator+(Matrix4 matrix);
	Matrix4 operator-(Matrix4 matrix);
	Matrix4 operator-();
	Matrix4 operator*(Matrix4 matrix);
	Vector4 operator*(Vector4 vector);
	Matrix4 operator*(float scalar);
	Matrix4 operator/(float scalar);
	Matrix4 operator=(Matrix4 matrix);
	bool operator==(Matrix4 matrix);
	bool operator!=(Matrix4 matrix);
	friend std::ostream& operator<<(std::ostream& os, Matrix4 matrix);
	Matrix4 inverse();
	Matrix4 transpose();
	Matrix4 clean();
	~Matrix4();
	static Matrix4 ViewMatrix(Vector3 center, Vector3 eye, Vector3 up);
	static Matrix4 OrthoMatrix(float left, float right, float top, float bottom, float near, float far);
	static Matrix4 ProjectionMatrix(float fovy, float aspect, float zNear, float zFar);
	static Matrix4 RodriguesRotation(Vector3 axis, float angle);
};


#endif

