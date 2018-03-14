#include "Matrix3.h"
#include "Matrix2.h"
#include "Matrix4.h"


const float Matrix3::ConvertDegreesToRadians(float angle)
{
	return angle * M_PI / 180.0;
}

Matrix3 Matrix3::clean()
{
	for (int i = 0; i < 9; i++) {
		if (fabs(data[i]) < THRESHOLD){
			data[i] = 0;
		}
	}
	return *this;
}

Matrix3::Matrix3()
{
	*this =  Matrix3(0, 0, 0, 0, 0, 0, 0, 0, 0);
}

Matrix3::Matrix3(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3)
{
	data[0] = x1;
	data[1] = y1;
	data[2] = z1;
	data[3] = x2;
	data[4] = y2;
	data[5] = z2;
	data[6] = x3;
	data[7] = y3;
	data[8] = z3;

}

const Matrix3 Matrix3::IDENTITY(1, 0, 0, 0, 1, 0, 0, 0, 1);
const Matrix3 Matrix3::ZERO(0, 0, 0, 0, 0, 0, 0, 0, 0);

///Axis should be X,Y or Z. Angle is in radians.
Matrix3 Matrix3::ROTATE(AXIS axis, float angle) {
	if (axis == X) {
		return Matrix3(1, 0, 0, 0, cos(angle), sin(angle), 0, -sin(angle), cos(angle));
	}
	else if (axis == Y) {
		return Matrix3(cos(angle), 0, -sin(angle), 0, 1, 0, sin(angle), 0, cos(angle));
	}
	else if (axis == Z) {
		return Matrix3(cos(angle), sin(angle), 0, -sin(angle), cos(angle), 0, 0, 0, 1);
	}
	else {
		return IDENTITY;
	}
}

Matrix3 Matrix3::SCALE(float sx, float sy, float sz)
{
	return Matrix3(sx,0,0,0,sy,0,0,0,sz);
}

Matrix4 Matrix3::TRANSLATE(float x, float y, float z)
{
	return Matrix4::TRANSLATE(x, y, z);
}

Matrix3 Matrix3::ConvertFromMatrix4(Matrix4 matrix)
{
	return Matrix3(matrix.data[0], matrix.data[1], matrix.data[2], matrix.data[4], matrix.data[5], matrix.data[6], matrix.data[8], matrix.data[9], matrix.data[10]);
}

Matrix4 Matrix3::ConvertToMatrix4(Matrix3 matrix)
{
	return Matrix4(matrix.data[0], matrix.data[1], matrix.data[2], 0, matrix.data[3], matrix.data[4], matrix.data[5], 0, matrix.data[6], matrix.data[7], matrix.data[8], 0, 0, 0, 0, 1);
}

Matrix3::~Matrix3()
{
}

bool Matrix3::operator==(Matrix3 matrix)
{
	for (int i = 0; i < 9; i++) {
		if (fabs(data[i] - matrix.data[i]) >= THRESHOLD) {
			return false;
		}
	}
	return true;
}

bool Matrix3::operator!=(Matrix3 matrix)
{
	return !operator==(matrix);
}

Matrix3 Matrix3::operator=(Matrix3 matrix)
{
	for (int i = 0; i < 9; i++) {
		data[i] = matrix.data[i];
	}
	return *this;
}

Matrix3 Matrix3::operator+(Matrix3 matrix)
{
	Matrix3 result = Matrix3::ZERO;
	for (int i = 0; i < 9; i++) {
		result.data[i] = data[i] + matrix.data[i];
	}
	return result;
}

Matrix3 Matrix3::operator-(Matrix3 matrix)
{
	Matrix3 result = Matrix3::ZERO;
	for (int i = 0; i < 9; i++) {
		result.data[i] = data[i] - matrix.data[i];
	}
	return result;
}

Matrix3 Matrix3::operator*(float scalar)
{
	return Matrix3(data[0] * scalar, data[1] * scalar, data[2] * scalar, data[3] * scalar, data[4] * scalar, data[5] * scalar, data[6] * scalar, data[7] * scalar, data[8] * scalar);
}

Vector3 Matrix3::operator*(Vector3 vector)
{
	float x = data[0] * vector.coordinates[0] + data[3] * vector.coordinates[1] + data[6] * vector.coordinates[2];
	float y = data[1] * vector.coordinates[0] + data[4] * vector.coordinates[1] + data[7] * vector.coordinates[2];
	float z = data[2] * vector.coordinates[0] + data[5] * vector.coordinates[1] + data[8] * vector.coordinates[2];

	return Vector3(x, y, z);

}

Matrix3 Matrix3::operator*(Matrix3 matrix)
{
	Matrix3 result = Matrix3::ZERO;
	result.data[0] = data[0] * matrix.data[0] + data[3] * matrix.data[1] + data[6] * matrix.data[2];
	result.data[1] = data[1] * matrix.data[0] + data[4] * matrix.data[1] + data[7] * matrix.data[2];
	result.data[2] = data[2] * matrix.data[0] + data[5] * matrix.data[1] + data[8] * matrix.data[2];


	result.data[3] = data[0] * matrix.data[3] + data[3] * matrix.data[4] + data[6] * matrix.data[5];
	result.data[4] = data[1] * matrix.data[3] + data[4] * matrix.data[4] + data[7] * matrix.data[5];
	result.data[5] = data[2] * matrix.data[3] + data[5] * matrix.data[4] + data[8] * matrix.data[5];

	result.data[6] = data[0] * matrix.data[6] + data[3] * matrix.data[7] + data[6] * matrix.data[8];
	result.data[7] = data[1] * matrix.data[6] + data[4] * matrix.data[7] + data[7] * matrix.data[8];
	result.data[8] = data[2] * matrix.data[6] + data[5] * matrix.data[7] + data[8] * matrix.data[8];

	return result;
	
}
Matrix3 Matrix3::DualMatrix(Vector3 axis) {
	return Matrix3(0.0, axis.coordinates[2], -axis.coordinates[1], -axis.coordinates[2], 0.0f, axis.coordinates[0], axis.coordinates[1], -axis.coordinates[0], 0.0f);
}
Matrix3 Matrix3::operator/(float scalar)
{
	return Matrix3(data[0]/scalar, data[1]/scalar, data[2]/scalar, data[3]/scalar, data[4] / scalar, data[5]/scalar, data[6]/scalar, data[7]/scalar, data[8]/scalar);
}

Matrix3 Matrix3::MatrixOfMinors(Matrix3 matrix)
{
	Matrix2 m1 = Matrix2(matrix.data[4], matrix.data[5], matrix.data[7], matrix.data[8]);
	Matrix2 m2 = Matrix2(matrix.data[3], matrix.data[5], matrix.data[6], matrix.data[8]);
	Matrix2 m3 = Matrix2(matrix.data[3], matrix.data[4], matrix.data[6], matrix.data[7]);
	Matrix2 m4 = Matrix2(matrix.data[1], matrix.data[2], matrix.data[7], matrix.data[8]);
	Matrix2 m5 = Matrix2(matrix.data[0], matrix.data[2], matrix.data[6], matrix.data[8]);
	Matrix2 m6 = Matrix2(matrix.data[0], matrix.data[1], matrix.data[6], matrix.data[7]);
	Matrix2 m7 = Matrix2(matrix.data[1], matrix.data[2], matrix.data[4], matrix.data[5]);
	Matrix2 m8 = Matrix2(matrix.data[0], matrix.data[2], matrix.data[3], matrix.data[5]);
	Matrix2 m9 = Matrix2(matrix.data[0], matrix.data[1], matrix.data[3], matrix.data[4]);
	return Matrix3(m1.determinant(), m2.determinant(), m3.determinant(), m4.determinant(), m5.determinant(), m6.determinant(), m7.determinant(), m8.determinant(), m9.determinant());
}

float Matrix3::determinant()
{
	float downdiagonal1;
	float downdiagonal2;
	float downdiagonal3;
	float updiagonal1;
	float updiagonal2;
	float updiagonal3;

	downdiagonal1 = data[0] * data[4] * data[8];
	downdiagonal2 = data[3] * data[7] * data[2];
	downdiagonal3 = data[6] * data[1] * data[5];

	updiagonal1 = data[2] * data[4] * data[6];
	updiagonal2 = data[5] * data[7] * data[0];
	updiagonal3 = data[8] * data[1] * data[3];


	return downdiagonal1 + downdiagonal2 + downdiagonal3 - updiagonal1 - updiagonal2 - updiagonal3;

}

Matrix3 Matrix3::transpose()
{
	return Matrix3(data[0], data[3],data[6], data[1], data[4], data[7], data[2], data[5], data[8]);
}

Matrix3 Matrix3::inverse()
{
	float det = determinant();
	if (det == 0) {
		throw std::exception("Determinant is 0. Can't calculate inverse.");
	}
	Matrix3 minor = MatrixOfMinors(transpose());
	minor.data[1] = -minor.data[1];
	minor.data[3] = -minor.data[3];
	minor.data[5] = -minor.data[5];
	minor.data[7] = -minor.data[7];
	
	return minor * (1/det);
}


std::ostream& operator<<(std::ostream& os, const Matrix3 matrix) {
	os << "Matrix3" << std::endl;
	os << "[" << matrix.data[0] << "," << matrix.data[3] << "," << matrix.data[6] << "]" << std::endl;
	os << "[" << matrix.data[1] << "," << matrix.data[4] << "," << matrix.data[7] << "]" << std::endl;
	os << "[" << matrix.data[2] << "," << matrix.data[5] << "," << matrix.data[8] << "]";
	return os;
}


