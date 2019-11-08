#pragma once

#include "vector4.h"
#include "plane.h"
#include <system/camera.h>

namespace Math
{


struct ALIGN16 Ray
{
	Ray(Math::vector4& start, Math::vector4& dir, float l):
		startPoint(start), 
		direction(dir), 
		length(l)
	{
		
	}
	Ray():
		startPoint(vector4(0.0f, 0.0f, 0.0f, 1.0f)),
		direction(vector4(0.0f, 0.0f, 0.0f, 1.0f)),
		length(0)
	{
		
	}

	bool Intersection(plane& p, vector4& isp);

	static Ray CreateRayFromScreenPos(vector2& pos, Camera* cam);
	Math::vector4 startPoint;
	Math::vector4 direction;
	float length;
};

inline bool Ray::Intersection(plane& p, vector4& isp)
{
	vector4 dir = this->direction * this->length;
	vector4 endPoint = this->startPoint + dir;
	return p.intersection(this->startPoint, endPoint, isp);
}

inline Ray Ray::CreateRayFromScreenPos(vector2& pos, Camera* cam)
{
	vector4 rayEye, temp;
	
	vector2 screenCoords = cam->NormalizeScreenCoordinates(pos);
	vector4 screen = vector4(screenCoords.x(), screenCoords.y(), -1.0f, 1.0f);

	matrix44 projInv = cam->projMatrix.inverse();
	rayEye = matrix44::transform(screen, projInv);
	rayEye.set_z(-1.0f);
	rayEye.set_w(1.0f);

	matrix44 origin = cam->viewTransformMatrix;
	vector4 rayWorld;
	temp = matrix44::transform(rayEye, cam->GetViewMatrix());
	rayWorld = vector4(temp.x(), temp.y(), temp.z());
	rayWorld = vector4::normalize3(rayWorld);

	vector4 rayStart = origin.getPos();
	vector4 rayDir = vector4(rayWorld.x(), rayWorld.y(), rayWorld.z());

	return Ray(rayStart, rayDir, 100.0f);
}
}

