#include "config.h"
#include "Joint.h"

Joint::Joint() : 
	parentJoint(nullptr),
	index(0),
	parentIndex(0),
	selected(false)
{

}

Joint::~Joint()
{
	this->parentJoint = nullptr;
	this->childJoints.clear();
}

void Joint::RotateJoint(Math::matrix44 rotMatrix)
{	
	this->jointMatrix = rotMatrix * this->superBaseMatrix ;
	this->jointMatrix = this->jointMatrix * this->parentJoint->GetJointMatrix();
	for (int i = 0; i < int(this->childJoints.size()); i++)
	{
		this->childJoints[i]->UpdateTransform();		
	}
}

void Joint::UpdateTransform()
{
	this->jointMatrix = this->superBaseMatrix * this->parentJoint->GetJointMatrix();
	for (int i = 0; i < int(this->childJoints.size()); i++)
	{
		this->childJoints[i]->UpdateTransform();
	}
}

void Joint::TranslateBaseMatrix(Math::matrix44 transMat)
{
	this->translatedMatrix = this->superBaseMatrix * transMat;
	this->jointMatrix = this->translatedMatrix;
	for(int i = 0; i < int(this->childJoints.size()); i++)
	{
		this->childJoints[i]->TranslateBaseMatrix(this->translatedMatrix);
	}
}

void Joint::ResetSelfAndChildren()
{
	this->jointMatrix = this->translatedMatrix;
	for(int i = 0; i < int(this->childJoints.size()); i++)
	{
		this->childJoints[i]->ResetSelfAndChildren();
	}
}

void Joint::ComputeBaseMatrix()
{
	this->superBaseMatrix = this->scale * this->rotation;
	this->superBaseMatrix[3][0] = this->position[3][0];
	this->superBaseMatrix[3][1] = this->position[3][1];
	this->superBaseMatrix[3][2] = this->position[3][2];
	//Math::matrix44 transpose = this->baseMatrix.transpose();
	int i = 0;
}
