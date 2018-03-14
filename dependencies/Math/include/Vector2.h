#pragma once
#include <math.h>
#include <iostream>
#include <sstream>

#ifndef VECTOR2_H
#define VECTOR2_H
#ifndef THRESHOLD
#define THRESHOLD 0.0005
#endif

class Vector2
{
public:
	float coordinates[2];
	float x;
	float y;
	Vector2();
	Vector2(float, float);
	~Vector2();

	Vector2 clear();

	inline Vector2 operator+(const Vector2 vector) const {
		return Vector2((coordinates[0] + vector.coordinates[0]), (coordinates[1] + vector.coordinates[1]));
	}

	inline Vector2 operator-(const Vector2 vector) const {
		return Vector2((coordinates[0] - vector.coordinates[0]), (coordinates[1] - vector.coordinates[1]));
	}

	inline bool operator==(const Vector2 vector) const {
		return (fabs(coordinates[0] - vector.coordinates[0]) < THRESHOLD) && (fabs(coordinates[1] - vector.coordinates[1]) < THRESHOLD);
	}

	inline bool operator!=(const Vector2 vector) const {
		return !operator==(vector);
	}
	
	inline float operator*(Vector2 vector) const {
		return (coordinates[0] * vector.coordinates[0]) + (coordinates[1] * vector.coordinates[1]);
	}

	friend inline Vector2 operator*(float c, Vector2 vector) {
		return vector * c;
	}

	Vector2 operator*(float c) const {
		return Vector2(c*coordinates[0], c*coordinates[1]);
	}
	Vector2 operator/(float c) const {
		return operator*(1 / c);
	}

	inline Vector2 operator=(const Vector2 vector) {
		coordinates[0] = vector.coordinates[0];
		coordinates[1] = vector.coordinates[1];
		return *this;
	}

	inline float quadrance() {
		return ((coordinates[0] * coordinates[0]) + (coordinates[1] * coordinates[1]));
	}

	inline float magnitude() {
		return sqrt(quadrance());
	}

	friend inline std::ostream& operator<<(std::ostream& os, Vector2 vector) {
		os << "Vector2[x:" << vector.coordinates[0] << ", y:" << vector.coordinates[1] << "]";
		return os;
	}
	
	friend inline std::istream& operator>>(std::istream& is, Vector2& vector) {
		is >> vector.coordinates[0];
		is >> vector.coordinates[1];
		return is;
	}

	Vector2 normalize();
	//get each coordinate as X and Y;

	float getX() const;
	float getY() const;
};

#endif
