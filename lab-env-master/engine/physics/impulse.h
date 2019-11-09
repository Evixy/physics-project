#pragma once

#include <math/vector4.h>
#include <physics\force.h>

using namespace Math;

#define IMPULSE_DURATION 0.1f // time in seconds

namespace System {
	class Entity;
}

namespace Physics
{
class Impulse
{
public:
	Impulse();
	Impulse(Force& force);
	
	Physics::Force GetForce();
	vector4 CalculateAccelerationFromImpulse(System::Entity* entity);


	void operator=(Impulse& newImpulse);

	double impulseStart;
	double impulseEnd;
	Force forceToApply;

private:
	
};
}