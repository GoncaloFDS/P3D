#pragma once
#include "Vector3.h"
#include "Vector2.h"
#include "Ray.h"

class Camera
{
public:
	Camera() = default;
	~Camera() = default;
	Vector3 Eye;
	Vector3 At;
	Vector3 Up;
	Vector2 Resolution;
	float Near; //Hither	
	float Far;	//Yon
	float Fovy;
	float GetResX();
	float GetResY();
	float Width;
	float Height;
	Vector3 Xe, Ye, Ze;
	virtual Ray CalculatePrimaryRay(int x, int y);
	void CalculateOtherStuff();
};

