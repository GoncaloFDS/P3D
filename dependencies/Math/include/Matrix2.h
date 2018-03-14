#pragma once

#ifndef MATRIX2_H
#define MATRIX2_H
#include "Vector2.h"
class Matrix3;
class Matrix2 {

public:

	float data[4];
	Matrix2();
	Matrix2(float x1, float y1, float x2 , float y2);
	const static Matrix2 IDENTITY;
	const static Matrix2 ZERO;
	static Matrix2 ROTATE(float angle);
	static Matrix2 SCALE(float sx, float sy);
	static Matrix3 TRANSLATION(float x, float y);
	static Matrix2 ConvertFromMatrix3(Matrix3 matrix);
	static Matrix3 ConvertToMatrix3(Matrix2 matrix);
	bool operator==(Matrix2 matrix);
	bool operator!=(Matrix2 matrix);
	Matrix2 operator+(Matrix2 matrix);
	Matrix2 operator-(Matrix2 matrix);
	Matrix2 operator*(float scalar);
	Vector2 operator*(Vector2 vector);
	Matrix2 operator*(Matrix2 matrix);
	Matrix2 operator=(Matrix2 matrix);
	Matrix2 operator/(float scalar);
	Matrix2 tranpose();
	float determinant();
	Matrix2 inverse();
	Matrix2 clean();
	friend std::ostream& operator<<(std::ostream& os, Matrix2 matrix);

	~Matrix2();






};













#endif