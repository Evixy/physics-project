#pragma once
#include <render/meshresource.h>
#include <math/matrix44.h>
#include <render/textureresource.h>
#include <render/shaderobject.h>

#include <physics/boundingbox.h>

#include "animation/Skeleton.h"
#include <physics/impulse.h>

namespace Physics
{
	struct Force;
	class Impulse;
}

class Camera;
using namespace Render;
namespace System
{
class ALIGN16 Entity
{
public:

	Entity();
	virtual ~Entity();
	void UpdateAI(double deltaTime);
	
	void* operator new(size_t i);
	void operator delete(void* p);
	void Draw();

	virtual void SetupShader(Camera* cam);
	void Update(double deltaTime);
	void UpdatePhysics(double deltaTime);
	void UpdatePos(double deltaTime);
	void UpdateRotation(double deltaTime);
	void UpdateBoundingBoxes();
	void SetBoundingBoxes();

	MeshResource* GetMesh();
	void SetMesh(MeshResource* m);

	TextureResource* GetTex();
	void SetTex(TextureResource* t);

	ShaderObject* GetShader();
	void SetShader(ShaderObject* s);

	matrix44& GetModelMatrix();
	void SetModelMatrix(const matrix44& trans);
	matrix44& GetTransformMatrix();
	Physics::BoundingBox& GetAABB();
	Physics::BoundingBox& GetOBB();

	void SetPosition(vector4& pos);

	void SetTransformMatrix(const matrix44& trans);
	void SetSelected(bool val);
	bool IsSelected() const;

	vector4 & GetVelocityVec();
	void SetVelocityVec(vector4& v);

	vector4 & GetAccelerationVec();
	void SetAccelerationVec(vector4& v);

	void ApplyImpulse(Physics::Impulse& impulse);

	double GetMass();
	void SetMass(double mass);

	matrix44 CalculateInertiaTensorFromOBB();
	vector4 CalculateCenterOfMass();

	vector4 & GetAngularMomentum();
	void SetAngularMomentum(vector4& v);

	matrix44& GetRotationMatrix();
	void SetRotationMatrix(const matrix44& m);

	void SetSpinVector(vector4& spin);
	vector4& GetSpinVector();

	void ApplyForce(Physics::Force& f);
	bool ForceApplied();
	void ForceHandledCallback();
	Physics::Force& GetForce();
	
	void DrawSkeleton();

	vector4 GetFurthestPointInDir(vector4& dir);

	double mass;

	vector4 acceleration;
	vector4 velocity; 
	vector4 angularMomentum;	
	vector4 spinVector;
	
	Physics::Impulse impulse;
	Physics::Force force;

	Physics::BoundingBox obb_original;
	Physics::BoundingBox obb_modified;
	Physics::BoundingBox aabb;

	ShaderObject* shader;
	MeshResource* mesh;
	TextureResource* tex;

	Skeleton* skeleton;
	MeshResource* jointBall;

	matrix44 modelMatrix;
	matrix44 transform;
	matrix44 rotation;
	matrix44 scale;
	bool selected;
	bool forceApplied;
	std::string name;
};

inline MeshResource* Entity::GetMesh()
{
	return this->mesh;
}

inline void Entity::SetMesh(MeshResource* m)
{
	this->mesh = m;
}

inline TextureResource* Entity::GetTex()
{
	return this->tex;
}

inline void Entity::SetTex(TextureResource* t)
{
	this->tex = t;
}

inline ShaderObject* Entity::GetShader()
{
	return this->shader;
}

inline void Entity::SetShader(ShaderObject* s)
{
	this->shader = s;
}

inline matrix44& Entity::GetModelMatrix()
{
	this->modelMatrix = this->rotation * this->transform * this->scale;
	//this->modelMatrix = this->scale * this->transform * this->rotation;
	return this->modelMatrix;
}

inline void Entity::SetModelMatrix(const matrix44& m)
{
	this->modelMatrix = m;
}
inline matrix44& Entity::GetTransformMatrix()
{
	return this->transform;
}

inline void Entity::SetTransformMatrix(const matrix44& m)
{
	this->transform = m;
}

inline void Entity::SetSelected(bool val)
{
	this->selected = val;
}

inline bool Entity::IsSelected() const
{
	return this->selected;
}

inline vector4 & Entity::GetVelocityVec()
{
	return this->velocity;
}

inline void Entity::SetVelocityVec(vector4& v)
{
	this->velocity = v;
}

inline vector4 & Entity::GetAccelerationVec()
{
	return this->acceleration;
}

inline void Entity::SetAccelerationVec(vector4& v)
{
	this->acceleration = v;
}

inline void Entity::ApplyImpulse(Physics::Impulse& impulse)
{
	this->impulse = impulse;
}

inline double Entity::GetMass()
{
	assert(this->mass > 0.000000f);
	return this->mass;
}

inline void Entity::SetMass(double mass)
{
	this->mass = mass;
}

inline vector4 & Entity::GetAngularMomentum()
{
	return this->angularMomentum;
}

inline void Entity::SetAngularMomentum(vector4& v)
{
	this->angularMomentum = v;
}

inline matrix44& Entity::GetRotationMatrix()
{
	return this->rotation;
}

inline void Entity::SetRotationMatrix(const matrix44& m)
{
	this->rotation = m;
}

inline void Entity::SetSpinVector(vector4& spin)
{
	this->spinVector = spin;
}

inline vector4& Entity::GetSpinVector()
{
	return this->spinVector;
}
}
