#pragma once
#include <GL/glew.h>
#include <math/matrix44.h>
#include <math/vector2.h>

using namespace Math;

class Camera
{
public:
	Camera(int screenWidth, int screenHeight);
	Camera(int screenWidth, int screenHeight, float xPos, float yPos, float zPos);
	~Camera();
	void* operator new(size_t i);
	void operator delete(void* p);

	void CalculateViewTransformMatrix();
	vector2 NormalizeScreenCoordinates(vector2& pos);
	vector4 ScreenToWorld(vector2& pos);
	vector2 WorldToScreen(vector4& pos);
	vector4 GetPos();
	void TranslateCamera(vector4& vec);

	matrix44 & GetViewMatrix();

	matrix44 viewTransformMatrix;	
	matrix44 projMatrix;

	float aspectRatio;
	float nearZ;
	float farZ;
	float fov;

	float screenWidth, screenHeight;
	float xRot, yRot, zRot;

	// Used for turning the camera. 
	// THIS IS NOT THE POSITION OF THE CAMERA. USE GetPos() FUNCTION INSTEAD
	float start_x, start_y, start_z;
	bool viewMatrixDirty;
private:
	matrix44 viewMatrix;
};

