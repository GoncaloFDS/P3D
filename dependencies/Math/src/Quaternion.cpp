#pragma once
#include "Quaternion.h"
typedef Vector4 vec4;
typedef Matrix4 mat4;
#define PI 3.14159265358979323846
#define ConvertToRadians(X) (X)*PI/180.0f
#define ConvertToDegrees(X) (X)*180.0f/PI
Quaternion::Quaternion()
{
	this->x = 0;
	this->y = 0;
	this->z = 0;
	this->t = 0;

}

Quaternion::~Quaternion()
{

}

Quaternion::Quaternion(float t, float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->t = t;
}

Quaternion::Quaternion(float radianAngle, Vector4 axis)
{
	auto normalized = axis.normalize();
	Quaternion q;
	q.t = cos(radianAngle / 2.0f);
	float s = sin(radianAngle / 2.0f);
	q.x = normalized.coordinates[0] * s;
	q.y = normalized.coordinates[1] * s;
	q.z = normalized.coordinates[2] * s;
	q.Clean();
}

Quaternion Quaternion::FromAngleAxis(const float thetaAngle, Vector4 axis)
{
	auto normalized = axis.normalize();
	Quaternion q;
	float radianAngle = ConvertToRadians(thetaAngle);
	q.t = cos(radianAngle / 2.0f);
	float s = sin(radianAngle / 2.0f);
	q.x = normalized.coordinates[0] * s;
	q.y = normalized.coordinates[1] * s;
	q.z = normalized.coordinates[2] * s;
	q.Clean();


	return q.NormalizeQuaternion();

}

const void Quaternion::QuaternionToAngleAxis(float& theta, Vector4& axis)
{
	Quaternion qn = this->NormalizeQuaternion();
	theta = ConvertToDegrees(2.0f*acosf(qn.t));
	float s = sqrt(1.0f - qn.t*qn.t);
	if(s < THRESHOLD)
	{
		axis.coordinates[0] = 1.0f;
		axis.coordinates[1] = 1.0f;
		axis.coordinates[2] = 0.0f;
		axis.coordinates[3] = 1.0f;
	}
	axis.coordinates[0] = qn.x / s;
	axis.coordinates[1] = qn.y / s;
	axis.coordinates[2] = qn.z / s;
	axis.coordinates[3] = 1.0f;
}

const float Quaternion::QuaternionQuadrance()
{
	return this->t * this->t + this->x * this->x + this->y * this->y + this->z * this->z;
}

const float Quaternion::QuaternionNorm()
{
	return sqrt(this->QuaternionQuadrance());
}

Quaternion Quaternion::NormalizeQuaternion()
{
	float s = 1 / this->QuaternionNorm();
	return *this *s;
}

const Quaternion Quaternion::Conjugate()
{
	return Quaternion(this->t, -this->x, -this->y, -this->z);
}

const Quaternion Quaternion::InverseQuaternion()
{
	Quaternion qConj = this->Conjugate();
	float quad = this->QuaternionQuadrance();
	return qConj * (1.0f / quad);
}

Quaternion Quaternion::operator+(Quaternion q1)
{
	float t = this->t + q1.t;
	float x = this->x + q1.x;
	float y = this->y + q1.y;
	float z = this->z + q1.z;
	return Quaternion(t, x, y, z);

}

Quaternion Quaternion::operator*(Quaternion q1)
{
	float t = this->t * q1.t - this->x * q1.x - this->y * q1.y - this->z * q1.z;
	float x = this->t * q1.x + this->x * q1.t + this->y * q1.z - this->z * q1.y;
	float y = this->t * q1.y + this->y * q1.t + this->z * q1.x - this->x * q1.z;
	float z = this->t * q1.z + this->z * q1.t + this->x * q1.y - this->y * q1.x;
	return Quaternion(t, x, y, z);
}

Quaternion Quaternion::operator*(const float factor)
{
	float t = factor * this->t;
	float x = factor * this->x;
	float y = factor * this->y;
	float z = factor * this->z;

	return Quaternion(t, x, y, z);
}

Quaternion Quaternion::operator=(Quaternion q1)
{
	this->t = q1.t;
	this->x = q1.x;
	this->y = q1.y;
	this->z = q1.z;
	return *this;
}

Matrix4 Quaternion::QuaternionToGLMatrix()
{
	Quaternion qn = this->NormalizeQuaternion();
	float xx = qn.x * qn.x;
	float xy = qn.x * qn.y;
	float xz = qn.x * qn.z;
	float xt = qn.x * qn.t;
	float yy = qn.y * qn.y;
	float yz = qn.y * qn.z;
	float yt = qn.y * qn.t;
	float zz = qn.z * qn.z;
	float zt = qn.z * qn.t;

	Matrix4 matrix = Matrix4::ZERO;
	matrix.data[0] = 1.0f - 2.0f * (yy + zz);
	matrix.data[1] = 2.0f * (xy + zt);
	matrix.data[2] = 2.0f * (xz - yt);
	matrix.data[3] = 0.0f;

	matrix.data[4] = 2.0f * (xy - zt);
	matrix.data[5] = 1.0f - 2.0f * (xx + zz);
	matrix.data[6] = 2.0f * (yz + xt);
	matrix.data[7] = 0.0f;

	matrix.data[8] = 2.0f * (xz + yt);
	matrix.data[9] = 2.0f * (yz - xt);
	matrix.data[10] = 1.0f - 2.0f * (xx + yy);
	matrix.data[11] = 0.0f;

	matrix.data[12] = 0.0f;
	matrix.data[13] = 0.0f;
	matrix.data[14] = 0.0f;
	matrix.data[15] = 1.0f;

	matrix.clean();
	return matrix;
}

Quaternion Quaternion::Lerp(Quaternion q0, Quaternion q1, float k)
{
	float cos_angle = q0.x*q1.x + q0.y * q1.y + q0.z * q1.z + q0.t * q1.t;
	float k0 = 1.0f - k;
	float k1 = (cos_angle > 0) ? k : -k;
	Quaternion q = q0 * k0 + q1 * k1;
	return q.NormalizeQuaternion();

}

Quaternion Quaternion::Slerp(Quaternion q0, Quaternion q1, float k)
{
	float angle = acos(q0.x*q1.x + q0.y*q1.y + q0.z*q1.z + q0.t*q1.t);
	float k0 = sin((1 - k)*angle) / sin(angle);
	float k1 = sin(k*angle) / sin(angle);
	Quaternion q = q0 * k0 + q1*k1;
	return q.NormalizeQuaternion();
}

bool Quaternion::operator==(Quaternion q1)
{
	return (fabs(this->t - q1.t) < THRESHOLD &&
			fabs(this->x - q1.x) < THRESHOLD &&
			fabs(this->y - q1.y) < THRESHOLD &&
			fabs(this->z - q1.z) < THRESHOLD);
}

bool Quaternion::operator!=(Quaternion q2) {
	return !operator==(q2);
}

Quaternion Quaternion::Clean()
{
	if (fabs(this->t) < THRESHOLD) this->t = 0.0f;
	if (fabs(this->x) < THRESHOLD) this->x = 0.0f;
	if (fabs(this->y) < THRESHOLD) this->y = 0.0f;
	if (fabs(this->z) < THRESHOLD) this->z = 0.0f;
	return *this;
}

void Quaternion::PrintAngleAxis(std::string s)
{
	std::cout << s.c_str() << " = [" << std::endl;

	float thetaf;
	vec4 axis_f;
	this->QuaternionToAngleAxis(thetaf, axis_f);
	std::cout << "  angle = " << thetaf << std::endl;
	std::cout << "  axis " << axis_f << std::endl;
	std::cout << "]" << std::endl;
}

Matrix4 Quaternion::ToGLMatrix()
{
	Quaternion qn = this->NormalizeQuaternion();
	float xx = qn.x * qn.x;
	float xy = qn.x * qn.y;
	float xz = qn.x * qn.z;
	float xt = qn.x * qn.t;
	float yy = qn.y * qn.y;
	float yz = qn.y * qn.z;
	float yt = qn.y * qn.t;
	float zz = qn.z * qn.z;
	float zt = qn.z * qn.t;

	Matrix4 mat4 = Matrix4::ZERO;
	mat4.data[0] = 1.0f - 2.0f * (yy + zz);
	mat4.data[1] = 2.0f * (xy + zt);
	mat4.data[2] = 2.0f * (xz - yt);
	mat4.data[3] = 0.0f;

	mat4.data[4] = 2.0f * (xy - zt);
	mat4.data[5] = 1.0f - 2.0f *(xx + zz);
	mat4.data[6] = 2.0f * (yz + xt);
	mat4.data[7] = 0.0f;

	mat4.data[8] = 2.0f * (xz + yt);
	mat4.data[9] = 2.0f * (yz - xt);
	mat4.data[10] = 1.0f - 2.0f*(xx + yy);
	mat4.data[11] = 0.0f;

	mat4.data[12] = 0.0f;
	mat4.data[13] = 0.0f;
	mat4.data[14] = 0.0f;
	mat4.data[15] = 1.0f;

	mat4.clean();
	return mat4;

}

std::ostream& operator<<(std::ostream& os, Quaternion q1)
{
	os << "Quaternion(t: " << q1.t << ", x:" << q1.x << ", y:" << q1.y << ", z:" << q1.z << ") @ 0x" << &q1 << std::endl;
	return os; 
}
