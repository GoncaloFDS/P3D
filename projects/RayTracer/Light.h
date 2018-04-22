#pragma once
#include "Vector3.h"

class Light {
public:
	Light(Vector3 pos, Vector3 color) : Position(pos), Color(color) {
		SampleSize = 1;
	};

	Light(Vector3 pos, Vector3 color, int samples) 
		: Position(pos), Color(color), SampleSize(samples) {};

	~Light() = default;

	virtual Vector3 getPoint() { return Position; }
	virtual void setPosition(Vector3 pos) { Position = pos; }
	Vector3 getColor() { return Color; }
	void setColor(Vector3 c) { Color = c; }
	int getSampleSize() { return SampleSize; }

protected:
	Vector3 Position;
	Vector3 Color;
	int SampleSize = 1;
};