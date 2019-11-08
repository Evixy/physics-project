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
#include <physics/collisiontest.h>
#include <physics/physicsserver.h>
#include <system/entitymanager.h>

using namespace Math;
using namespace System;
using namespace Physics;

class PhysicsProject : public Core::App
{
public:

	PhysicsProject();
	~PhysicsProject();

	bool Open();
	void InitializeInput();
	void LoadWorld();
	void CreateCube(float mass, vector4& pos, matrix44& rot, std::string name);
	void CreateQuad(float mass, vector4& pos);
	void CreateRayBalls();
	
	void DrawRay();
	void DrawAABBs();
	void DrawEntities();
	void UpdateEntities(double deltaTime);
	void Run();

	void RenderUI();

	void RenderNano(NVGcontext* vg);
		
	void SetCamera(Camera * camera);
	Camera* GetCamera();

	void CheckRayCollission(vector2& screenPos, float dir);
	void Initialize();
	void Start();
	
	void DrawCoordinateSystem();


private:

	GLuint program;
	GLuint vertexShader;
	GLuint pixelShader;
	GLuint triangle;
	GLchar *vsBuffer;
	GLchar *fsBuffer;
	std::string compilerLog;
	PhysicsServer* physServer;
	Camera* camera;
	ShaderObject* lineShader;
	EntityManager* entityManager;	
	CollisionTest* collisionTest;
	Entity* ballHit;
	Entity* ballStart;
	Display::Window* window;
	Ray ray;
	float mousePos_x;
	float mousePos_y;

	bool hasRay;
	bool rightMouseIsDown;
	bool leftMouseIsDown;
	bool leftShiftIsDown;
	bool middleMouseIsDown;
};

//------------------------------------------------------------------------------
/**
*/
inline void PhysicsProject::SetCamera(Camera * camera)
{
	this->camera = camera;
}

//------------------------------------------------------------------------------
/**
*/
inline Camera* PhysicsProject::GetCamera()
{
	return this->camera;
}
