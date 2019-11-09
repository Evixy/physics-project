#include "config.h"
#include "physicsserver.h"
#include "force.h"
#include "impulse.h"
#include <GLFW\glfw3.h>
#include <system/entity.h>
using namespace System;

namespace Physics {
Impulse::Impulse()
{
	// empty
}

Impulse::Impulse(Force& force):
	forceToApply(force)
{
	this->impulseStart = glfwGetTime();
	this->impulseEnd = this->impulseStart + IMPULSE_DURATION;
}

Physics::Force Impulse::GetForce()
{
	double currentTime = glfwGetTime();

	// Check impulse is over. 
	if (currentTime > impulseEnd)
	{
		return Physics::Force();
	}
	return this->forceToApply;
}

vector4 Impulse::CalculateAccelerationFromImpulse(System::Entity* entity)
{
	Force force = this->GetForce();
	vector4 acceleration = force.force / float(entity->GetMass());

	return acceleration;
}


void Impulse::operator=(Impulse& newImpulse)
{
	// Get force applied from existing impulse
	Force f = this->GetForce();

	// Add force from existing impulse and new impulse to create a new force
	this->forceToApply.force += newImpulse.forceToApply.force;
	this->impulseStart = newImpulse.impulseStart;
	this->impulseEnd = newImpulse.impulseEnd;
}
}

