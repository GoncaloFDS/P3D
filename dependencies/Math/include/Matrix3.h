#pragma once

#ifndef MATRIX3_H
#define MATRIX3_H
#define _USE_MATH_DEFINES
#define THRESHOLD 0.0005
#include <cmath>
#include "Vector3.h"

class Matrix2;
class Matrix4;

enum AXIS {
	X, Y, Z
};

class Matrix3
{

public:

	const float static ConvertDegreesToRadians(float angle);
	Matrix3 clean();
	float data[9];
	Matrix3();
	Matrix3(float,float,float,float,float,float,float,float,float);
	~Matrix3();
	const static Matrix3 IDENTITY;
	const static Matrix3 ZERO;
	static Matrix3 ROTATE(AXIS axis, float angle);
	static Matrix3 SCALE(float sx, float sy, float sz);
	static Matrix4 TRANSLATE(float x, float y, float z);
	static Matrix3 ConvertFromMatrix4(Matrix4 matrix);
	static Matrix4 ConvertToMatrix4(Matrix3 matrix);
	bool operator==(Matrix3 matrix);
	bool operator!=(Matrix3 matrix);
	Matrix3 operator=(Matrix3 matrix);
	Matrix3 operator+(Matrix3 matrix);
	Matrix3 operator-(Matrix3 matrix);
	Matrix3 operator*(float scalar);
	Vector3 operator*(Vector3 vector);
	Matrix3 operator*(Matrix3 matrix);
	static Matrix3 DualMatrix(Vector3 axis);
	Matrix3 operator/(float scalar);
	friend std::ostream& operator<<(std::ostream&, const Matrix3 matrix);
	static Matrix3 MatrixOfMinors(Matrix3 matrix);
	float determinant();
	Matrix3 transpose();
	Matrix3 inverse();


	
};

#endif