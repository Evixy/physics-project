#pragma once
#include <math/vector4.h>

using namespace Math;

namespace Physics
{
struct Force
{
	Force()	
	{
		
	}
	void CalculateForce(vector4& acceleration, float mass, float accelerationMagnitude = 1.0f);
	float MagnitudeOfForce();
	vector4 force;
	vector4 hitPoint;
};

inline void Force::CalculateForce(vector4& acceleration, float mass, float accelerationMagnitude)
{
	vector4 acc = acceleration * accelerationMagnitude;
	this->force = acc * mass;
}

inline float Force::MagnitudeOfForce()
{
	return this->force.length3();
}
}
