#pragma once

class MathUtils{
public:
	static inline float clamp(float x, float min, float max) {
		return (x < min ? min : (x > max ? max : x));
	}
};