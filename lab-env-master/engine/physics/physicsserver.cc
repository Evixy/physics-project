#include "config.h"
#include <float.h>
#include "physicsserver.h"
#include <math/miscmath.h>
#include "force.h"
#include <physics\impulse.h>
using namespace System;

namespace Physics
{

//------------------------------------------------------------------------------
/**
*/
PhysicsServer::PhysicsServer()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
PhysicsServer* PhysicsServer::Instance()
{
	static PhysicsServer inst;

	return &inst;
}

//------------------------------------------------------------------------------
/**
*/
bool PhysicsServer::RayTrace(Ray& ray, std::vector<Collision>& hits, std::vector<Entity*>& worldObjects)
{
	for (uint i = 0; i < worldObjects.size(); i++)
	{
		Entity* ent = worldObjects[i];
		BoundingBox box = ent->GetAABB();
		assert(box.GetBoundingType() == AABB);
		if (box.BoxIntersectsRay(ray))
		{
			for (uint j = 0; j < ent->GetMesh()->vb.size(); j += 3)
			{
				matrix44 trans;
				vector4 v1 = ent->GetMesh()->vb[j].pos;
				vector4 v2 = ent->GetMesh()->vb[j + 1].pos;
				vector4 v3 = ent->GetMesh()->vb[j + 2].pos;

				trans = matrix44::transMatFromVec(v1);
				v1 = (trans * ent->GetModelMatrix()).getPos();

				trans = matrix44::transMatFromVec(v2);
				v2 = (trans * ent->GetModelMatrix()).getPos();

				trans = matrix44::transMatFromVec(v3);
				v3 = (trans * ent->GetModelMatrix()).getPos();

				plane p = plane(v1, v2, v3);

				vector4 isp;
				if (ray.Intersection(p, isp))
				{
					isp.set_w(0.0f);
					if (PointInTriangle(isp, v1, v2, v3))
					{
						Collision c;
						c.hitPoint = isp;
						c.entity = ent;
						hits.push_back(c);
					}
				}
			}
		}
	}

	if (hits.size() > 0)
	{
		return true;
	}
	return false;
}

//------------------------------------------------------------------------------
/**
*/
matrix44 PhysicsServer::CalculateIntertiaTensor(Entity* ent)
{
	matrix44 mat;
	float w, h, d;
	double m;
	BoundingBox obb = ent->GetOBB();
	vector4 dimensions = obb.GetBoxDimensions();
	w = dimensions.x();
	h = dimensions.y();
	d = dimensions.z();

	m = ent->GetMass();
	float c = 1 / 12.0f;

	float Ix = float(c * m * (h * h + d * d));
	float Iy = float(c * m * (w * w + d * d));
	float Iz = float(c * m * (w * w + h * h));

	mat[0][0] = Ix;
	mat[1][1] = Iy;
	mat[2][2] = Iz;

	return mat;
}

//------------------------------------------------------------------------------
/**
*/
vector4 PhysicsServer::CalculateTorque(Entity* entity, Force& force)
{
	vector4 centerMass = entity->CalculateCenterOfMass();
	vector4 temp = force.hitPoint - centerMass;
	vector4 torque = vector4::cross3(force.force, temp);
	return torque;
}

//------------------------------------------------------------------------------
/**
*/
Collision & PhysicsServer::GetClosestCollision(vector4& startPoint, std::vector<Collision> & hits)
{
	float dist = FLT_MAX;
	uint closestHit = 0;
	for (uint i = 0; i < hits.size(); i++)
	{
		vector4 vec = hits[i].hitPoint - startPoint;
		float vecLength = vec.length3();
		if (dist > vecLength)
		{
			dist = vecLength;
			closestHit = i;
		}
	}
	return hits[closestHit];
}

//------------------------------------------------------------------------------
/**
*/
void PhysicsServer::ApplyForceToEntity(Entity* entity, Force& force)
{			
	Impulse impulse = Impulse(force);
	entity->ApplyImpulse(impulse);
	entity->impulse;

	vector4 torque = PhysicsServer::CalculateTorque(entity, force);
	vector4 angularMomentum = entity->GetAngularMomentum();
	angularMomentum = angularMomentum + torque;
	entity->SetAngularMomentum(angularMomentum);

	//Spin vector
	matrix44 inertiaTensor = entity->CalculateInertiaTensorFromOBB();
	matrix44 invInertiaTensor = matrix44::inverse(inertiaTensor);
	vector4 spin = invInertiaTensor * angularMomentum;
	entity->SetSpinVector(spin);
}

//------------------------------------------------------------------------------
/**
*/
void PhysicsServer::UpdatePhysics(std::vector<Entity*> entities, float deltaTime)
{
	for(uint i = 0; i < entities.size(); i++)
	{
		entities[i]->UpdatePhysics(deltaTime);
	}
}

void PhysicsServer::UpdateBoundingBoxes(std::vector<System::Entity*>& entities)
{
	for(uint i = 0; i < entities.size(); i++)
	{
		entities[i]->UpdateBoundingBoxes();
	}
}
}

/*
    vector3D force;
    force[0] = r.dir[0] * this->force;
    force[1] = r.dir[1] * this->force;
    force[2] = r.dir[2] * this->force;

    vector3D acc;

    acc = force * this->invmass;

    this->velocity = this->velocity + (acc * deltaTime);
    this->position = this->position + (this->velocity * deltaTime);

    trans = trans*trans.transmat(this->position[0],this->position[1],this->position[2]);

    inversInertia = this->rotation * matrix4D::inverse(calculateInertiaTensor(2,2,2)) * ~this->rotation;

    this->angulareMomentum = this->angulareMomentum + this->torque;

    this->spin = inversInertia * angulareMomentum;
    this->spin = this->spin * deltaTime;

    matrix4D skewrotation = matrix4D::skewMatrix(this->spin) * this->rotation;

    this->rotation = this->rotation + skewrotation;

    this->force = 0.0f;
    this->torque = vector3D(0,0,0);

    vector3D xaxis, yaxis, zaxis;

    xaxis.setvalues(rotation[0][0], rotation[0][1], rotation[0][2]);
    yaxis.setvalues(rotation[1][0], rotation[1][1], rotation[1][2]);
    zaxis.setvalues(rotation[2][0], rotation[2][1], rotation[2][2]);

    xaxis = vector3D::normalize(xaxis);
    yaxis = vector3D::normalize(yaxis);
    zaxis = vector3D::normalize(zaxis);

    rotation[0][0] = xaxis[0];
    rotation[0][1] = xaxis[1];
    rotation[0][2] = xaxis[2];

    rotation[1][0] = yaxis[0];
    rotation[1][1] = yaxis[1];
    rotation[1][2] = yaxis[2];

    rotation[2][0] = zaxis[0];
    rotation[2][1] = zaxis[1];
    rotation[2][2] = zaxis[2];

    return this->rotation * trans; //this is a model matrix, eg this->rotation is the rotation matrix for the rotating object
 */