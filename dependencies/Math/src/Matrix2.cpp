#include "Matrix2.h"
#include "Matrix3.h"

const Matrix2 Matrix2::IDENTITY(1, 0, 0, 1);
const Matrix2 Matrix2::ZERO(0, 0, 0, 0);

Matrix2::Matrix2()
{
	data[0] = 0;
	data[1] = 0;
	data[2] = 0;
	data[3] = 0;
}

Matrix2::Matrix2(float a, float b, float c, float d)
{
	data[0] = a;
	data[1] = b;
	data[2] = c;
	data[3] = d;
}

Matrix2 Matrix2::ROTATE(float angle)
{
	return Matrix2(cos(angle), sin(angle), -sin(angle), cos(angle));
}

Matrix2 Matrix2::SCALE(float sx, float sy)
{
	return Matrix2(sx,0,0,sy);
}

Matrix3 Matrix2::TRANSLATION(float x, float y)
{
	return Matrix3(1,0,0,0,1,0,x,y,1);
}

Matrix2 Matrix2::ConvertFromMatrix3(Matrix3 matrix)
{
	return Matrix2(matrix.data[0], matrix.data[1], matrix.data[3], matrix.data[4]);
}

Matrix3 Matrix2::ConvertToMatrix3(Matrix2 matrix)
{
	return Matrix3(matrix.data[0], matrix.data[1], 0, matrix.data[2], matrix.data[3], 0,0,0,1);	
}

bool Matrix2::operator==(Matrix2 matrix)
{
	for (int i = 0; i < 4; i++) {
		if (fabs(data[i] - matrix.data[i]) >= THRESHOLD) {
			return false;
		}
	}
	return true;
}

bool Matrix2::operator!=(Matrix2 matrix)
{
	return !operator==(matrix);
}

Matrix2 Matrix2::operator+(Matrix2 matrix)
{
	return Matrix2(data[0] + matrix.data[0], data[1] + matrix.data[1], data[2] + matrix.data[2], data[3]+matrix.data[3]);
}

Matrix2 Matrix2::operator-(Matrix2 matrix)
{
	return Matrix2(data[0] - matrix.data[0], data[1] - matrix.data[1], data[2] - matrix.data[2], data[3] - matrix.data[3]);
}

Matrix2 Matrix2::operator*(float scalar)
{
	return Matrix2(data[0] * scalar, data[1] * scalar, data[2] * scalar, data[3] * scalar);
}

Vector2 Matrix2::operator*(Vector2 vector)
{
	float x = data[0] * vector.coordinates[0] + data[2] * vector.coordinates[1];
	float y = data[1] * vector.coordinates[0] + data[3] * vector.coordinates[1];

	return Vector2(x,y);
}

Matrix2 Matrix2::operator*(Matrix2 matrix)
{
	float a = data[0] * matrix.data[0] + data[2] * matrix.data[1];
	float c = data[1] * matrix.data[0] + data[3] * matrix.data[1];
	float b = data[0] * matrix.data[2] + data[2] * matrix.data[3];
	float d = data[1] * matrix.data[2] + data[3] * matrix.data[3];


	return Matrix2(a,c,b,d);
}

Matrix2 Matrix2::operator=(Matrix2 matrix)
{
	data[0] = matrix.data[0];
	data[1] = matrix.data[1];
	data[2] = matrix.data[2];
	data[3] = matrix.data[3];

	return *this;
}

Matrix2 Matrix2::operator/(float scalar)
{
	return operator*(1/scalar);
}

Matrix2 Matrix2::tranpose()
{
	return Matrix2(data[0], data[2], data[1], data[3]);
}

float Matrix2::determinant()
{
	return data[0] * data[3] - data[1] * data[2];
}

Matrix2 Matrix2::inverse()
{

	float det = determinant();
	if (det == 0) {
		throw std::exception("Can't inverse when determinant is 0.");
	}
	
	return Matrix2(data[3]/det, -data[1]/det, -data[2]/det, data[0]/det);
}

Matrix2 Matrix2::clean()
{
	for (int i = 0; i < 4; i++) {
		if (fabs(data[i]) < THRESHOLD) {
			data[i] = 0;
		}
	}
	return *this;
}

Matrix2::~Matrix2()
{
}

std::ostream & operator<<(std::ostream & os, Matrix2 matrix)
{
	os << "Matrix" << std::endl;
	os << "[" << matrix.data[0] << "," << matrix.data[2] << "]" << std::endl;
	os << "[" << matrix.data[1] << "," << matrix.data[3] << "]";
	return os;
}
