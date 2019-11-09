#include "config.h"
#include "entity.h"
#include "camera.h"
#include <physics/force.h>
#include "animation/Joint.h"
#include <physics/impulse.h>

namespace System
{

//------------------------------------------------------------------------------
/**
*/
Entity::Entity():

skeleton(nullptr),
selected(false),
forceApplied(false)
{
}

//------------------------------------------------------------------------------
/**
*/
Entity::~Entity()
{
}



//------------------------------------------------------------------------------
/**
*/
void* Entity::operator new(size_t i)
{
	return _mm_malloc(i, 16);
}

//------------------------------------------------------------------------------
/**
*/
void Entity::operator delete(void* p)
{
	_mm_free(p);
}

//------------------------------------------------------------------------------
/**
*/
void Entity::Draw()
{
	this->mesh->DrawObject();
}

//------------------------------------------------------------------------------
/**
*/
void Entity::SetupShader(Camera* cam)
{
	matrix44 viewMatrix = cam->GetViewMatrix();
	matrix44 modelMatrix = this->GetModelMatrix();
	vector4 color = vector4(0.0f, 0.0f, 0.0f);
	this->shader->Bind();
	this->shader->setUniformMatrix4fv(this->shader->proj_in, cam->projMatrix);
	this->shader->setUniformMatrix4fv(this->shader->view_in, viewMatrix);
	this->shader->setUniformMatrix4fv(this->shader->model_in, modelMatrix);

	if(this->IsSelected())
	{
		color = vector4(1.0f, 0.0f, 0.0f);
	}
	this->shader->setUniformVector4fv(this->shader->color_in, color);
}

void Entity::Update(double deltaTime)
{
	this->UpdatePhysics(deltaTime);
	this->UpdateAI(deltaTime);
	this->UpdateBoundingBoxes();
}

void Entity::UpdatePhysics(double deltaTime)
{
	this->UpdatePos(deltaTime);
	this->UpdateRotation(deltaTime);	
}

void Entity::UpdatePos(double deltaTime)
{
	//Updating position through velocity via acceleration
	this->acceleration = this->impulse.CalculateAccelerationFromImpulse(this);
	vector4 newVelocity = this->acceleration * float(deltaTime);
	this->velocity = this->velocity + newVelocity;
	matrix44 transMat = matrix44::transMatFromVec(this->velocity);
	this->transform = this->transform * transMat;
}

void Entity::UpdateRotation(double deltaTime)
{
	//Updating rotation
	vector4 tempSpin = this->spinVector * float(deltaTime); 
	matrix44 spinSkewMatrix = matrix44::createSkewMatrix(tempSpin);
	matrix44 tempRotation = this->rotation;
	matrix44 skewRotation = spinSkewMatrix * this->rotation;
	this->rotation = skewRotation + this->rotation;
	this->rotation.normalizeAxis();
}

//------------------------------------------------------------------------------
/**
*/
void Entity::UpdateAI(double deltaTime)
{

}

//------------------------------------------------------------------------------
/**
*/
void Entity::UpdateBoundingBoxes()
{
	this->obb_modified = this->obb_original;
	matrix44 model = this->GetModelMatrix();
	vector4 pos = model.getPos();
	this->obb_modified.MultiplyBoundsWithMatrix(this->GetModelMatrix());
	this->aabb.DefineBoundsByBox(this->obb_modified);
}

//------------------------------------------------------------------------------
/**
*/
void Entity::SetBoundingBoxes()
{
	this->obb_original.SetBoundingType(Physics::OBB);
	this->obb_original.DefineBoundsByMesh(this->mesh);
	this->obb_modified = this->obb_original;
	this->aabb = this->obb_modified;
	this->aabb.SetBoundingType(Physics::AABB);
}

//------------------------------------------------------------------------------
/**
*/
Physics::BoundingBox& Entity::GetAABB()
{
	this->UpdateBoundingBoxes();
	return this->aabb;
}

//------------------------------------------------------------------------------
/**
*/
Physics::BoundingBox &Entity::GetOBB()
{
    return this->obb_original;
}

//------------------------------------------------------------------------------
/**
*/
void Entity::SetPosition(vector4& pos)
{
	this->transform.setPos(pos);	
}

//------------------------------------------------------------------------------
/**
*/
Math::matrix44 Entity::CalculateInertiaTensorFromOBB()
{
	matrix44 iTensor;
	vector4 v = this->obb_modified.GetBoxDimensions();
	float c = float(1.0f / 12.0f * this->mass);
	
	float x = v.x();
	float y = v.y();
	float z = v.z();

	iTensor[0][0] = c * (y * y + z * z);
	iTensor[1][1] = c * (x * x + z * z);
	iTensor[2][2] = c * (x * x + y * y);
	iTensor[3][3] = 0.0f;
	vector4 cm = this->CalculateCenterOfMass();
	return iTensor;
}

//------------------------------------------------------------------------------
/**
*/
vector4 Entity::CalculateCenterOfMass()
{
	float m = float(this->mass / 8.0f);
	vector4 v;
	vector4 temp;
	for (int i = 0; i < 8; i++)
	{
		temp = temp + (this->obb_modified.bounds[i] * m);
	}
	v = temp * float(1 / this->mass);
	return v;
}

//------------------------------------------------------------------------------
/**
*/
void Entity::ApplyForce(Physics::Force& f)
{
	this->force = f;
	this->forceApplied = true;
}

//------------------------------------------------------------------------------
/**
*/
bool Entity::ForceApplied()
{
	return this->forceApplied;
}

//------------------------------------------------------------------------------
/**
*/
void Entity::ForceHandledCallback()
{	
	this->forceApplied = false;
}

//------------------------------------------------------------------------------
/**
*/
Physics::Force& Entity::GetForce()
{
	return this->force;
}

void Entity::DrawSkeleton()
{
	vector4 color(1.0f, 1.0f, 1.0f, 1.0f);
	this->shader->setUniformVector4fv("color_in", color);
	glBegin(GL_LINES);
	for (uint i = 0; i < int(this->skeleton->joints.size()); i++)
	{		
		vector4 start;
		vector4 end;
		start = this->skeleton->joints[i]->jointMatrix.getPos();
		if(this->skeleton->joints[i]->parentJoint)
		{
			end = this->skeleton->joints[i]->parentJoint->jointMatrix.getPos();
		}		
		glVertex3f(start[0], start[1], start[2]);
		glVertex3f(end[0], end[1], end[2]);		
	}
	
	glEnd();
	for (uint i = 0; i < int(this->skeleton->joints.size()); i++)
	{
		vector4 pos = this->skeleton->joints[i]->jointMatrix.getPos();
		matrix44 trans = matrix44::transMatFromVec(pos);
		if(this->skeleton->joints[i]->selected)
		{
			color = vector4(1.0f, 0.0f, 0.0f, 1.0f);
		}
		else
		{
			color = vector4(1.0f, 1.0f, 1.0f, 1.0f);
		}
		this->shader->setUniformMatrix4fv("model_in", trans);
		this->shader->setUniformVector4fv("color_in", color);
	}
}

vector4 Entity::GetFurthestPointInDir(vector4& dir)
{	
	matrix44 modelMatrix = this->GetModelMatrix();
	vector4 furthestVertex = modelMatrix * this->mesh->vb[0].pos;
	float furthestDistance = vector4::dot3(furthestVertex, dir);
	
	for (uint i = 1; i < this->mesh->vb.size(); i++)
	{
		vector4 transformedVertex = modelMatrix * this->mesh->vb[i].pos;
		float val = vector4::dot3(transformedVertex, dir);
		
		if (furthestDistance < val)
		{
			furthestVertex = transformedVertex;
			furthestDistance = val;
		}
		
	}
	return furthestVertex;
}
}
