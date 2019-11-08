#include "config.h"
#include "Skeleton.h"
#include "Joint.h"

Skeleton::Skeleton():
	rootJoint(nullptr)
{
}

Skeleton::~Skeleton()
{
	for(int i = 0; i < int(this->joints.size()); i++)
	{
		delete this->joints[i];
	}
	this->joints.clear();
}

void Skeleton::AppendJoint(Joint* joint)
{
	this->joints.push_back(joint);
}

void Skeleton::SetJoints(std::vector<Joint*>& joints)
{
	this->joints = joints;
}

Joint* Skeleton::GetRootJoint()
{
	if(this->rootJoint != nullptr)
	{
		return this->rootJoint;
	}

	for (int i = 0; i < int(this->joints.size()); i++)
	{
		if(this->joints[i]->GetParent() == nullptr)
		{
			this->rootJoint = this->joints[i];
			return this->rootJoint;
		}
	}
	assert(false); // ERROR: no root node found in skeleton
	return nullptr;
}

void Skeleton::RotateJointByIndex(unsigned int index, Math::matrix44 mat)
{
	this->joints[index]->RotateJoint(mat);
}

std::vector<Joint*> & Skeleton::GetJoints()
{
	return this->joints;
}

void Skeleton::ResetJoints()
{
	this->GetRootJoint()->ResetSelfAndChildren();
}
