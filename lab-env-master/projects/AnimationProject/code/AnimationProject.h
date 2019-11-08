#pragma once
#include "config.h"
#include "render/window.h"
#include "core/app.h"
#include <memory>
#include <render/textureresource.h>
#include <render/meshresource.h>
#include <render/shaderobject.h>
#include <system/camera.h>
#include <system/entity.h>
#include <math/ray.h>
#include <physics/collision.h>
#include <physics/physicsserver.h>
#include <system/entitymanager.h>

using namespace Math;
using namespace System;
using namespace Physics;

class AnimationProject : public Core::App
{
public:

	AnimationProject();
	~AnimationProject();

	bool Open();
	void InitializeInput();
	void LoadWorld();
	void DrawRay();
	void DrawAABBs();
	void DrawEntities();
	void UpdatePhysics(double deltaTime);
	void UpdateAI(double deltaTime);
	void UpdateEntities(double deltaTime);
	void Run();
	void RotateJoints();

	void SetCamera(Camera * camera);
	Camera* GetCamera();


	void CheckCollission(vector2& screenPos, float dir);
	void LoadSkeleton();
	void Initialize();
	void Start();
	
	//void DrawCoordinateSystem();

private:

	PhysicsServer* physServer;
	Camera* camera;
	ShaderObject* lineShader;
	EntityManager* entityManager;	
	Entity* ballHit;
	Entity* ballStart;
	Display::Window* window;
	Ray ray;
	float mousePos_x;
	float mousePos_y;
	std::vector<Entity*> entities;
	matrix44 rotYMatrix;
	matrix44 rotXMatrix;
	matrix44 jointRotationX;
	matrix44 jointRotationY;
	matrix44 jointRotationZ;
	matrix44 modelMatrix;
	matrix44 transMatrix;
	matrix44 projMatrix;
	matrix44 viewMatrix;

	Skeleton* skeleton;
	Joint* currentJoint;
	Joint* currentParentJoint;
	int currentChildIndex;



	const char* id_model = "model_in";
	const char* id_proj = "proj_in";
	const char* id_view = "view_in";
	const char* id_color = "color_in";

	bool rotateX;
	bool rotateY;
	bool rotateZ;
	bool drawSkeleton;

	bool hasRay;
	bool rightMouseIsDown;
	bool leftMouseIsDown;
	bool leftShiftIsDown;
	bool middleMouseIsDown;
};

//------------------------------------------------------------------------------
/**
*/
inline void AnimationProject::SetCamera(Camera * camera)
{
	this->camera = camera;
}

//------------------------------------------------------------------------------
/**
*/
inline Camera* AnimationProject::GetCamera()
{
	return this->camera;
}
