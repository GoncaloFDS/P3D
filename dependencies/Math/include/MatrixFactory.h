#pragma once
#include "Matrix2.h"
#include "Matrix3.h"
#include "Matrix4.h"
class MatrixFactory
{
public:
	MatrixFactory();
	~MatrixFactory();
	static Matrix3 InverseMat3(Matrix3 toInvert);
	static Matrix3 TransposeMat3(Matrix3 toTranspose);
	static Matrix4 TranslateMat4(float x, float y, float z);
	static Matrix4 RotationMat4(float angle, Vector3 axis);
	static Matrix4 ScalingMat4(float x, float y, float z);
	static Matrix4 InverseMat4(Matrix4 toInvert);
	static Matrix4 TranposeMat4(Matrix4 toTranspose);

	static Matrix3 Mat4ToMat3(Matrix4 matrix);
	static Matrix4 Mat3ToMat4(Matrix3 matrix);

};

