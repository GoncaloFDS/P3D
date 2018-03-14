#pragma once
#include "Vector3.h"
#include "Vector2.h"
#include "../../Ray.h"

class Camera
{
public:
	Camera();
	Camera(Vector3 eye, Vector3 at, Vector3 up, double fovy, double near, double far, int ResX, int ResY);
	~Camera();
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
	Ray CalculatePrimaryRay(int x, int y);
	void CalculateOtherStuff();
};

