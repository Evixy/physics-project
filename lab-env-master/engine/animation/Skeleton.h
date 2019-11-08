#pragma once

#include <vector>
#include "Math/matrix44.h"

class Joint;

class Skeleton
{
public:
	Skeleton();
	~Skeleton();
	void AppendJoint(Joint* joint);
	void SetJoints(std::vector<Joint*>& joints);
	Joint* GetRootJoint();
	void RotateJointByIndex(unsigned index, Math::matrix44);
	std::vector<Joint*>& GetJoints();
	void ResetJoints();

	std::vector<Joint*> joints;
	Joint* rootJoint;
};

