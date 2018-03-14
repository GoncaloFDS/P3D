#include "MatrixFactory.h"



MatrixFactory::MatrixFactory()
{
}


MatrixFactory::~MatrixFactory()
{
}

Matrix3 MatrixFactory::InverseMat3(Matrix3 toInvert)
{
	return toInvert.inverse();
}

Matrix3 MatrixFactory::TransposeMat3(Matrix3 toTranspose)
{
	return toTranspose.transpose();
}


Matrix4 MatrixFactory::TranslateMat4(float x, float y, float z)
{
	return Matrix4::TRANSLATE(x, y, z);
}

Matrix4 MatrixFactory::RotationMat4(float angle, Vector3 axis)
{
	return Matrix4::RodriguesRotation(axis, angle);
}

Matrix4 MatrixFactory::ScalingMat4(float x, float y, float z)
{
	return Matrix4::SCALE(x, y, z);
}


Matrix3 MatrixFactory::Mat4ToMat3(Matrix4 matrix)
{
	return Matrix3::ConvertFromMatrix4(matrix);
}

Matrix4 MatrixFactory::Mat3ToMat4(Matrix3 matrix)
{
	return Matrix3::ConvertToMatrix4(matrix);
}

