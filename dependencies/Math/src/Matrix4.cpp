#include "Matrix4.h"
#include "Matrix3.h"
#include "Vector3.h"

Matrix4::Matrix4()
{
}
const Matrix4 Matrix4::ZERO(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
const Matrix4 Matrix4::IDENTITY(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
Matrix4::Matrix4(float x1, float y1, float z1, float w1, float x2, float y2, float z2, float w2, float x3, float y3, float z3, float w3, float x4, float y4, float z4, float w4)
{
	data[0] = x1;
	data[1] = y1;
	data[2] = z1;
	data[3] = w1;

	data[4] = x2;
	data[5] = y2;
	data[6] = z2;
	data[7] = w2;
	
	data[8] = x3;
	data[9] = y3;
	data[10] = z3;
	data[11] = w3;

	data[12] = x4;
	data[13] = y4;
	data[14] = z4;
	data[15] = w4;
}

Matrix4 Matrix4::ROTATE(AXIS axis, float angle)
{
	return Matrix3::ConvertToMatrix4(Matrix3::ROTATE(axis, angle));
}

Matrix4 Matrix4::SCALE(float x, float y, float z)
{
	return Matrix3::ConvertToMatrix4(Matrix3::SCALE(x,y,z));
}

Matrix4 Matrix4::TRANSLATE(float x, float y, float z)
{
	return Matrix4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, x, y, z, 1);;
}

Matrix4 Matrix4::operator+(Matrix4 matrix)
{
	return Matrix4(data[0]+matrix.data[0], data[1] + matrix.data[1], data[2] + matrix.data[2], data[3] + matrix.data[3], data[4] + matrix.data[4],
		data[5] + matrix.data[5], data[6] + matrix.data[6], data[7] + matrix.data[7], data[8] + matrix.data[8], data[9] + matrix.data[9],
		data[10] + matrix.data[10], data[11] + matrix.data[11], data[12] + matrix.data[12], data[13] + matrix.data[13], data[14] + matrix.data[14], data[15] + matrix.data[15]);
}

Matrix4 Matrix4::operator-(Matrix4 matrix)
{
	return Matrix4(data[0] - matrix.data[0], data[1] - matrix.data[1], data[2] - matrix.data[2], data[3] - matrix.data[3], data[4] - matrix.data[4],
		data[5] - matrix.data[5], data[6] - matrix.data[6], data[7] - matrix.data[7], data[8] - matrix.data[8], data[9] - matrix.data[9],
		data[10] - matrix.data[10], data[11] - matrix.data[11], data[12] - matrix.data[12], data[13] - matrix.data[13], data[14] - matrix.data[14], data[15] - matrix.data[15]);
}

Matrix4 Matrix4::operator-()
{
	Matrix4 minus = Matrix4::ZERO;
	for (int i = 0; i < 15; i++) {
		minus.data[i] = -this->data[i];
	}
	return minus;
}

Matrix4 Matrix4::operator*(Matrix4 matrix)
{
	Matrix4 result = Matrix4::ZERO;
	result.data[0] = data[0] * matrix.data[0] + data[4] * matrix.data[1] + data[8] * matrix.data[2] + data[12] * matrix.data[3];
	result.data[1] = data[1] * matrix.data[0] + data[5] * matrix.data[1] + data[9] * matrix.data[2] + data[13] * matrix.data[3];
	result.data[2] = data[2] * matrix.data[0] + data[6] * matrix.data[1] + data[10] * matrix.data[2] + data[14] * matrix.data[3];
	result.data[3] = data[3] * matrix.data[0] + data[7] * matrix.data[1] + data[11] * matrix.data[2] + data[15] * matrix.data[3];

	result.data[4] = data[0] * matrix.data[4] + data[4] * matrix.data[5] + data[8] * matrix.data[6] + data[12] * matrix.data[7];
	result.data[5] = data[1] * matrix.data[4] + data[5] * matrix.data[5] + data[9] * matrix.data[6] + data[13] * matrix.data[7];
	result.data[6] = data[2] * matrix.data[4] + data[6] * matrix.data[5] + data[10] * matrix.data[6] + data[14] * matrix.data[7];
	result.data[7] = data[3] * matrix.data[4] + data[7] * matrix.data[5] + data[11] * matrix.data[6] + data[15] * matrix.data[7];

	result.data[8] = data[0] * matrix.data[8] + data[4] * matrix.data[9] + data[8] * matrix.data[10] + data[12] * matrix.data[11];
	result.data[9] = data[1] * matrix.data[8] + data[5] * matrix.data[9] + data[9] * matrix.data[10] + data[13] * matrix.data[11];
	result.data[10] = data[2] * matrix.data[8] + data[6] * matrix.data[9] + data[10] * matrix.data[10] + data[14] * matrix.data[11];
	result.data[11] = data[3] * matrix.data[8] + data[7] * matrix.data[9] + data[11] * matrix.data[10] + data[15] * matrix.data[11];

	result.data[12] = data[0] * matrix.data[12] + data[4] * matrix.data[13] + data[8] * matrix.data[14] + data[12] * matrix.data[15];
	result.data[13] = data[1] * matrix.data[12] + data[5] * matrix.data[13] + data[9] * matrix.data[14] + data[13] * matrix.data[15];
	result.data[14] = data[2] * matrix.data[12] + data[6] * matrix.data[13] + data[10] * matrix.data[14] + data[14] * matrix.data[15];
	result.data[15] = data[3] * matrix.data[12] + data[7] * matrix.data[13] + data[11] * matrix.data[14] + data[15] * matrix.data[15];

	return result;
}

Vector4 Matrix4::operator*(Vector4 vector)
{
	Vector4 result = Vector4();
	result.coordinates[0] = data[0] * vector.coordinates[0] + data[4] * vector.coordinates[1] + data[8] * vector.coordinates[2] + data[12] * vector.coordinates[3];
	result.coordinates[1] = data[1] * vector.coordinates[0] + data[5] * vector.coordinates[1] + data[9] * vector.coordinates[2] + data[13] * vector.coordinates[3];
	result.coordinates[2] = data[2] * vector.coordinates[0] + data[6] * vector.coordinates[1] + data[10] * vector.coordinates[2] + data[14] * vector.coordinates[3];
	result.coordinates[3] = data[3] * vector.coordinates[0] + data[7] * vector.coordinates[1] + data[11] * vector.coordinates[2] + data[15] * vector.coordinates[3];

	return result;
}

Matrix4 Matrix4::operator*(float scalar)
{
	return Matrix4(data[0] * scalar, data[1] * scalar, data[2] * scalar, data[3] * scalar, data[4] * scalar, data[5] * scalar, data[6] * scalar, data[7] * scalar, data[8] * scalar,
					data[9] * scalar, data[10] * scalar, data[11] * scalar, data[12] * scalar, data[13] * scalar, data[14] * scalar, data[15]*scalar);
}

Matrix4 Matrix4::operator/(float scalar)
{
	return operator*(1 / scalar);
}

Matrix4 Matrix4::operator=(Matrix4 matrix)
{
	for (int i = 0; i < 16; i++) {
		data[i] = matrix.data[i];
	}
	return *this;
}

bool Matrix4::operator==(Matrix4 matrix)
{
	for (int i = 0; i < 16; i++) {
		if (fabs(data[i] - matrix.data[i]) >= THRESHOLD) {
			return false;
		}
	}
	return true;
}

bool Matrix4::operator!=(Matrix4 matrix)
{
	return !operator==(matrix);
}


Matrix4 Matrix4::transpose()
{
	Matrix4 result = Matrix4::ZERO;
	result.data[0] = data[0];
	result.data[1] = data[4];
	result.data[2] = data[8];
	result.data[3] = data[12];

	result.data[4] = data[1];
	result.data[5] = data[5];
	result.data[6] = data[9];
	result.data[7] = data[13];

	result.data[8] = data[2];
	result.data[9] = data[6];
	result.data[10] = data[10];
	result.data[11] = data[14];
	
	result.data[12] = data[3];
	result.data[13] = data[7];
	result.data[14] = data[11];
	result.data[15] = data[15];


	return result;
}

Matrix4 Matrix4::clean()
{
	for (int i = 0; i < 16; i++) {
		if (fabs(data[i]) < THRESHOLD) {
			data[i] = 0;
		}
	}


	return *this;
}

Matrix4::~Matrix4()
{
}

Matrix4 Matrix4::ViewMatrix(Vector3 center, Vector3 eye, Vector3 up)
{
	Vector3 V = (center - eye).normalize();
	Vector3 S = Vector3::crossProduct(V, up).normalize();
	Vector3 U = Vector3::crossProduct(S, V).normalize();

	return Matrix4(S.coordinates[0], U.coordinates[0], -V.coordinates[0], 0, S.coordinates[1], U.coordinates[1], -V.coordinates[1], 0, S.coordinates[2], U.coordinates[2], -V.coordinates[2], 0, (-S)*eye, (-U)*eye, V*eye, 1);

//	return Matrix4(S.coordinates[0], S.coordinates[1], S.coordinates[2], (-S)*eye, U.coordinates[0], U.coordinates[1], U.coordinates[2], (-U)*eye, -V.coordinates[0], -V.coordinates[1], -V.coordinates[2], V*eye, 0,0,0,1 );
	
}

Matrix4 Matrix4::OrthoMatrix(float left, float right, float top, float bottom, float near, float far)
{
	float x1 = 2 / (right - left);
	float y2 = 2 / (top - bottom);
	float z3 = 2 / (near - far);
	float x4 = (left + right) / (left - right);
	float y4 = (bottom + top) / (bottom - top);
	float z4 = (near + far) / (near - far);
	return Matrix4(x1,0,0,0,0,y2,0,0,0,0,z3,0,x4,y4, z4,1);
}

Matrix4 Matrix4::ProjectionMatrix(float fovy, float aspect, float zNear, float zFar)
{
	float theta = fovy / 2;
	float d = 1 / tan(theta);
	return Matrix4(d / aspect, 0, 0, 0, 0, d, 0, 0, 0, 0, (zNear + zFar) / (zNear - zFar), -1 , 0, 0, (2 * zNear*zFar) / (zNear - zFar), 0);
}

Matrix4 Matrix4::RodriguesRotation(Vector3 axis, float angle) {
	float cosine = cosf(angle);
	float sine = sinf(angle);


	Matrix3 A = Matrix3::DualMatrix(axis.normalize());
	Matrix3 id = Matrix3::IDENTITY;
	Matrix3 rodrigues = id + A * sine + (A * A) * (1 - cosine);

	return Matrix3::ConvertToMatrix4(rodrigues);


}
std::ostream & operator<<(std::ostream & os, Matrix4 matrix)
{
	os << "Matrix4" << std::endl;
	os << "[" << matrix.data[0] << "," << matrix.data[4] << "," << matrix.data[8] << "," << matrix.data[12] << "]" << std::endl;
	os << "[" << matrix.data[1] << "," << matrix.data[5] << "," << matrix.data[9] << "," << matrix.data[13] << "]" << std::endl;
	os << "[" << matrix.data[2] << "," << matrix.data[6] << "," << matrix.data[10] << "," << matrix.data[14] << "]" << std::endl;
	os << "[" << matrix.data[3] << "," << matrix.data[7] << "," << matrix.data[11] << "," << matrix.data[15] << "]";
	return os;
}
