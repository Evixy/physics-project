#include "PhysicsProject.h"
#include <string>
#include <cmath>
#include <chrono>
#include <math.h>
#include <stdio.h>
#include <system/objectloader.h>
#include <math/ray.h>
#include <math/miscmath.h>
#include <physics/collision.h>
#include <physics/physicsserver.h>
#include <physics/force.h>
#include <thread>
#include <chrono>
#include "physics/collisiontest.h"
#include "imgui.h"

#define STRING_BUFFER_SIZE 8192

using namespace Display;
using namespace Math;

//------------------------------------------------------------------------------
/**
*/
PhysicsProject::PhysicsProject() :
	window(nullptr),
	rightMouseIsDown(false),
	leftMouseIsDown(false),
	leftShiftIsDown(false),
	middleMouseIsDown(false),
	hasRay(false)
{		
	
}

//------------------------------------------------------------------------------
/**
*/
PhysicsProject::~PhysicsProject()
{
	delete this->camera;
}

void PhysicsProject::Initialize()
{
	this->physServer = PhysicsServer::Instance();
	this->entityManager = EntityManager::Instance();
	this->collisionTest = CollisionTest::Instance();
	this->lineShader = new ShaderObject("LineShader.vert", "LineShader.frag");
}

//------------------------------------------------------------------------------
/**
*/
void PhysicsProject::Start()
{
	if (this->Open())
	{
		this->InitializeInput();
		this->Initialize();
		this->LoadWorld();
		this->Run();
	}
	this->Exit();
}

void PhysicsProject::DrawCoordinateSystem()
{
	this->lineShader->Bind();
	this->lineShader->setUniformMatrix4fv(this->lineShader->proj_in, this->camera->projMatrix);
	this->lineShader->setUniformMatrix4fv(this->lineShader->view_in, this->camera->GetViewMatrix());
    vector4 col;
	col = vector4(1.0f, 1.0f, 1.0f, 1.0f);
	this->lineShader->setUniformVector4fv("color_in", col);
	glPointSize(10);
	glBegin(GL_POINTS);
	glVertex3f(0.0f, -0.207107f, -0.5f);
	glVertex3f(1.0f, 0.5f, 1.201701f);
	glVertex3f(0.0f, -0.207107f, 0.5f);
	glVertex3f(0.0f, 1.201701f, 0.5f);
	glEnd();

    //positive x-axis, red
    col = vector4(1.0f, 0.0f, 0.0f, 1.0f);
	this->lineShader->setUniformVector4fv("color_in", col);
	glBegin(GL_LINES);


	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(3.0f, 0.0f, 0.0f);

	glEnd();

	//positive x-axis, less red
	col = vector4(0.3f, 0.0f, 0.3f, 1.0f);
	this->lineShader->setUniformVector4fv("color_in", col);
	glBegin(GL_LINES);


	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-3.0f, 0.0f, 0.0f);

	glEnd();


	//positive y-axis, green
    col = vector4(0.0f, 1.0f, 0.0f, 1.0f);
	this->lineShader->setUniformVector4fv("color_in", col);
	glBegin(GL_LINES);

	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 3.0f, 0.0f);
	glEnd();

	//positive y-axis, less green
	col = vector4(0.3f, 0.3f, 0.0f, 1.0f);
	this->lineShader->setUniformVector4fv("color_in", col);
	glBegin(GL_LINES);

	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, -3.0f, 0.0f);
	glEnd();

	//positive z-axis, blue
    col = vector4(0.0f, 0.0f, 1.0f, 1.0f);
	this->lineShader->setUniformVector4fv("color_in", col);
	glBegin(GL_LINES);

	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 3.0f);

	glEnd();

	//negative z-axis, less blue
	col = vector4(0.0f, 0.3f, 0.3f, 1.0f);
	this->lineShader->setUniformVector4fv("color_in", col);
	glBegin(GL_LINES);

	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, -3.0f);

	glEnd();

}

void PhysicsProject::DrawRay()
{
	if (this->hasRay)
	{
        vector4 col = vector4(0.0f, 1.0f, 1.0f, 1.0f);
		this->lineShader->Bind();
		this->lineShader->setUniformMatrix4fv(this->lineShader->proj_in, this->camera->projMatrix);
		this->lineShader->setUniformMatrix4fv(this->lineShader->view_in, this->camera->GetViewMatrix());
		this->lineShader->setUniformVector4fv("color_in", col);
		glBegin(GL_LINES);

		glVertex3f(this->ray.startPoint.x(), this->ray.startPoint.y(), this->ray.startPoint.z());
		vector4 end = this->ray.startPoint + this->ray.direction * this->ray.length;
		glVertex3f(end.x(), end.y(), end.z());

		glEnd();
	}
}

void PhysicsProject::DrawAABBs()
{
    vector4 col = vector4(1.0f, 0.0f, 1.0f, 1.0f);
	this->lineShader->Bind();
	this->lineShader->setUniformMatrix4fv(this->lineShader->proj_in, this->camera->projMatrix);
	this->lineShader->setUniformMatrix4fv(this->lineShader->view_in, this->camera->GetViewMatrix());
	this->lineShader->setUniformVector4fv("color_in", col);
	std::vector<Entity*> entities = this->entityManager->GetEntities();
	for(uint i = 0; i < entities.size(); i++)
	{
		Physics::BoundingBox box = entities[i]->GetAABB();
		vector4 v1 = box.bounds[0];
		vector4 v2 = box.bounds[1];
		vector4 v3 = box.bounds[2];
		vector4 v4 = box.bounds[3];

		vector4 v5 = box.bounds[4];
		vector4 v6 = box.bounds[5];
		vector4 v7 = box.bounds[6];
		vector4 v8 = box.bounds[7];

		glBegin(GL_LINES);
		glVertex3f(v1.x(), v1.y(), v1.z());
		glVertex3f(v2.x(), v2.y(), v2.z());
		glEnd();

		glBegin(GL_LINES);
		glVertex3f(v1.x(), v1.y(), v1.z());
		glVertex3f(v4.x(), v4.y(), v4.z());
		glEnd();

		glBegin(GL_LINES);
		glVertex3f(v1.x(), v1.y(), v1.z());
		glVertex3f(v5.x(), v5.y(), v5.z());
		glEnd();

		glBegin(GL_LINES);
		glVertex3f(v2.x(), v2.y(), v2.z());
		glVertex3f(v3.x(), v3.y(), v3.z());
		glEnd();

		glBegin(GL_LINES);
		glVertex3f(v3.x(), v3.y(), v3.z());
		glVertex3f(v4.x(), v4.y(), v4.z());
		glEnd();

		glBegin(GL_LINES);
		glVertex3f(v4.x(), v4.y(), v4.z());
		glVertex3f(v8.x(), v8.y(), v8.z());
		glEnd();

		glBegin(GL_LINES);
		glVertex3f(v8.x(), v8.y(), v8.z());
		glVertex3f(v5.x(), v5.y(), v5.z());
		glEnd();

		glBegin(GL_LINES);
		glVertex3f(v5.x(), v5.y(), v5.z());
		glVertex3f(v6.x(), v6.y(), v6.z());
		glEnd();

		glBegin(GL_LINES);
		glVertex3f(v3.x(), v3.y(), v3.z());
		glVertex3f(v7.x(), v7.y(), v7.z());
		glEnd();

		glBegin(GL_LINES);
		glVertex3f(v6.x(), v6.y(), v6.z());
		glVertex3f(v7.x(), v7.y(), v7.z());
		glEnd();

		glBegin(GL_LINES);
		glVertex3f(v7.x(), v7.y(), v7.z());
		glVertex3f(v8.x(), v8.y(), v8.z());
		glEnd();

		glBegin(GL_LINES);
		glVertex3f(v6.x(), v6.y(), v6.z());
		glVertex3f(v2.x(), v2.y(), v2.z());
		glEnd();
	}
}

void PhysicsProject::DrawEntities()
{
	std::vector<Entity*> entities = this->entityManager->GetEntities();
	for(uint i = 0; i < entities.size(); i++)
	{
		Entity* ent = entities[i];
		ent->GetTex()->Bind();
		ent->SetupShader(this->camera);		
		ent->Draw();
	}
}

void PhysicsProject::UpdateEntities(double deltaTime)
{
	std::vector<Entity*> entities = this->entityManager->GetEntities();
	for (uint i = 0; i < entities.size(); i++)
	{
		entities[i]->Update(deltaTime);
	}
	this->collisionTest->CheckCollision(entities);
}

//------------------------------------------------------------------------------
/**
*/
bool PhysicsProject::Open()
{
	App::Open();

	this->window = new Display::Window;
	this->camera = new Camera(this->window->GetWidth(), this->window->GetHeight(), 0.0f, 0.0f, 2.0f);
	if (this->window->Open())
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		// set ui rendering function
		
		this->window->SetUiRender([this]()
			{
				this->RenderUI();
			});
		/*
		this->window->SetNanoVGRender([this](NVGcontext* vg)
			{
				this->RenderNano(vg);
			});
		*/
		return true;
	}
	return false;
}

//------------------------------------------------------------------------------
/**
*/
void PhysicsProject::InitializeInput()
{
	this->window->SetKeyPressFunction([this](int32 key, int32, int32 action, int32)
	{
		if (key == GLFW_KEY_ESCAPE)
		{
			this->window->Close();
		}

		if(key == GLFW_KEY_LEFT_SHIFT)
		{
			if (action == GLFW_REPEAT || action == GLFW_PRESS)
			{
				this->leftShiftIsDown = true;
			}
			else if( action == GLFW_RELEASE)
			{
				this->leftShiftIsDown = false;
			}
		}
		if (key == GLFW_KEY_W)
		{
			if (action == GLFW_REPEAT || action == GLFW_PRESS)
			{		
				//Move up
				vector4 v(0.0f, 0.1f, 0.0f);
				this->camera->TranslateCamera(v);				
			}
		}
		if (key == GLFW_KEY_S)
		{
			if (action == GLFW_REPEAT || action == GLFW_PRESS)
			{
				//Move down
				vector4 v(0.0f, -0.1f, 0.0f);
				this->camera->TranslateCamera(v);
			}
		}
		if (key == GLFW_KEY_A)
		{
			if (action == GLFW_REPEAT || action == GLFW_PRESS)
			{		
				//Move left
				vector4 v(-0.1f, 0.0f, 0.0f);
				this->camera->TranslateCamera(v);
			}
		}
		if (key == GLFW_KEY_D)
		{
			if (action == GLFW_REPEAT || action == GLFW_PRESS)
			{
				//Move right
				vector4 v(0.1f, 0.0f, 0.0f);
				this->camera->TranslateCamera(v);
			}				
		}       
		if(key == GLFW_KEY_R)
		{
			if(action == GLFW_PRESS)
			{
				this->camera->xRot = 0.0f;
				this->camera->yRot = 0.0f;
				this->camera->zRot = 0.0f;
			}
		}
		vector4 ray(0.0f, 0.0f, -1.0f);
		
		if (key == GLFW_KEY_1 && action == GLFW_PRESS)
		{
			// no rot (facing negative z-axis)
			this->camera->xRot = 0.0f;
			this->camera->yRot = 0.0f;
			this->camera->zRot = 0.0f;
			this->camera->CalculateViewTransformMatrix();
			vector4 trans = matrix44::transform(ray, this->camera->GetViewMatrix());
			printf("\n");
			printf("Facing negative z-axis \n");
			printf("RAY transformed (none-inv): (%f, %f, %f) \n", trans.x(), trans.y(), trans.z());
		}
		if (key == GLFW_KEY_2 && action == GLFW_PRESS)
		{
			// 180 degrees yRot (facing positive z-axis)
			this->camera->xRot = 0.0f;
			this->camera->yRot = PI;
			this->camera->zRot = 0.0f;
			this->camera->CalculateViewTransformMatrix();
			vector4 trans = matrix44::transform(ray, this->camera->GetViewMatrix());
			printf("\n");
			printf("Facing positive z-axis \n");
			printf("RAY transformed (none-inv): (%f, %f, %f) \n", trans.x(), trans.y(), trans.z());
		}
		if (key == GLFW_KEY_3 && action == GLFW_PRESS)
		{
			// 90 degrees yRot (facing negative x-azis)			
			this->camera->xRot = 0.0f;
			this->camera->yRot = PI/2.0f;
			this->camera->zRot = 0.0f;
			this->camera->CalculateViewTransformMatrix();
			vector4 trans = matrix44::transform(ray, this->camera->GetViewMatrix());
			printf("\n");
			printf("Facing positive x-axis \n");
			printf("RAY transformed (none-inv): (%f, %f, %f) \n", trans.x(), trans.y(), trans.z());
		}
		if (key == GLFW_KEY_4 && action == GLFW_PRESS)
		{
			// 270 degrees yRot (facing positive x-axis)
			this->camera->xRot = 0.0f;
			this->camera->yRot = PI + PI/2.0f;
			this->camera->zRot = 0.0f;
			this->camera->CalculateViewTransformMatrix();
			vector4 trans = matrix44::transform(ray, this->camera->GetViewMatrix());
			printf("\n");
			printf("Facing negative x-axis \n");
			printf("RAY transformed (none-inv): (%f, %f, %f) \n", trans.x(), trans.y(), trans.z());
		}
		if (key == GLFW_KEY_5 && action == GLFW_PRESS)
		{
			// 270 degrees yRot (facing positive x-axis)
			this->camera->xRot = PI + PI / 2.0f;
			this->camera->yRot = 0.0f;
			this->camera->zRot = 0.0f;
			this->camera->CalculateViewTransformMatrix();
			vector4 trans = matrix44::transform(ray, this->camera->GetViewMatrix());
			printf("\n");
			printf("Facing positive y-axis \n");
			printf("RAY transformed (none-inv): (%f, %f, %f) \n", trans.x(), trans.y(), trans.z());
		}
		if (key == GLFW_KEY_6 && action == GLFW_PRESS)
		{
			// 90 degrees yRot (facing positive x-axis)
			this->camera->xRot = PI/2.0f;
			this->camera->yRot = 0.0f;
			this->camera->zRot = 0.0f;
			this->camera->CalculateViewTransformMatrix();
			
			vector4 trans = matrix44::transform(ray, this->camera->GetViewMatrix());
			printf("\n");
			printf("Facing negative y-axis \n");
			printf("RAY transformed (none-inv): (%f, %f, %f) \n", trans.x(), trans.y(), trans.z());
		} 
		if (key == GLFW_KEY_F1 && action == GLFW_PRESS)
		{
			// Test case 1

			// Get entities
			Entity* entityFirst = this->entityManager->GetEntityByName(std::string("first"));
			Entity* entitySecond = this->entityManager->GetEntityByName(std::string("second"));
			
			// Set acceleration for entities
			entityFirst->SetAccelerationVec(vector4(0.0f, 0.0f, 0.0f));
			entitySecond->SetAccelerationVec(vector4(0.0f, 0.0f, 0.0f));

			// Set velocity for entities
			entityFirst->SetVelocityVec(vector4(0.0f, 0.0f, 0.0f));
			entitySecond->SetVelocityVec(vector4(0.0f, 0.0f, 0.0f));

			// Set spinvector for entities
			entityFirst->SetSpinVector(vector4(0.0f, 0.0f, 0.0f));
			entitySecond->SetSpinVector(vector4(0.0f, 0.0f, 0.0f));

			// Set position for entities
			entityFirst->SetPosition(vector4(0.0f, 0.0f, 0.0f));
			entitySecond->SetPosition(vector4(0.0f, 0.0f, 1.5f));

			// Set rotation for entities
			matrix44 rot_x = matrix44::rotation_x(45.0f, true);
			matrix44 rot_y = matrix44::rotation_y(45.0f, true);

			matrix44 rot_first = rot_x * rot_y;
			matrix44 rot_second = rot_x;
			
			entityFirst->SetRotationMatrix(rot_first);
			entitySecond->SetRotationMatrix(rot_second);

		}
	});

	this->window->SetMousePressFunction([this](int32 key, int32 action, int32)
	{
		if (key == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS)
		{
			this->leftMouseIsDown = true;
			//here we do ray cast
			vector2 mousePos(this->mousePos_x, this->mousePos_y);
			float dir = 1.0f;
			this->CheckRayCollission(mousePos, dir);
			
		}
		if (key == GLFW_MOUSE_BUTTON_1 && action == GLFW_RELEASE)
		{
			this->leftMouseIsDown = false;
		}
		if (key == GLFW_MOUSE_BUTTON_2 && action == GLFW_PRESS)
		{
			this->rightMouseIsDown = true;
			//here we do ray cast
			vector2 mousePos(this->mousePos_x, this->mousePos_y);
			float dir = -1.0f;
			this->CheckRayCollission(mousePos, dir);
		}
		if (key == GLFW_MOUSE_BUTTON_2 && action == GLFW_RELEASE)
		{
			this->rightMouseIsDown = false;
		}
		if (key == GLFW_MOUSE_BUTTON_3 && action == GLFW_PRESS)
		{
			this->middleMouseIsDown = true;
		}
		if (key == GLFW_MOUSE_BUTTON_3 && action == GLFW_RELEASE)
		{
			this->middleMouseIsDown = false;
		}
	});

	this->window->SetMouseMoveFunction([this](float64 x, float64 y)
	{
		this->mousePos_x = float(x);
		this->mousePos_y = float(y);

		if (this->middleMouseIsDown)
		{
			if (this->camera->start_x + 10 < x)
			{
				this->camera->yRot += 0.04f;
				this->camera->start_x = float(x);
			}
			else if (this->camera->start_x - 10 > x)
			{
				this->camera->yRot -= 0.04f;
				this->camera->start_x = float(x);
			}
			if (this->camera->start_y + 10 < y)
			{
				this->camera->xRot += 0.04f;
				this->camera->start_y = float(y);
			}
			else if (this->camera->start_y - 10 > y)
			{
				this->camera->xRot -= 0.04f;
				this->camera->start_y = float(y);
			}			
		}
	});

	this->window->SetMouseScrollFunction([this](float64 x, float64 y)
	{
		vector4 v(0.0f, 0.0f, -(float(y) / 10.0f));
		this->camera->TranslateCamera(v);
	});	    
}

void PhysicsProject::CheckRayCollission(vector2& screenPos, float dir)
{	
	Ray r = Ray::CreateRayFromScreenPos(screenPos, this->camera);
	
	this->ray = r;
	this->hasRay = true;
	std::vector<Physics::Collision> hits;
	matrix44 matStart = matrix44::transMat(r.startPoint);
	this->ballStart->SetTransformMatrix(matStart);
	
	//printf("\n");
	//printf("Ray Start:  (%f, %f, %f) \n", r.startPoint.x(), r.startPoint.y(), r.startPoint.z());
	//printf("Ray Dir:    (%f, %f, %f) \n", r.direction.x(), r.direction.y(), r.direction.z());
	//printf("Screen: pos     (%f, %f) \n", rayScreen.x(), rayScreen.y());
	//printf("Eye:    ray     (%f, %f, %f) \n", rayEye.x(), rayEye.y(), rayEye.z());
	//printf("Eye:    transf  (%f, %f, %f) \n", rayTemp.x(), rayTemp.y(), rayTemp.z());
	//printf("Ray:    start   (%f, %f, %f) \n", rayStart.x(), rayStart.y(), rayStart.z());
	//printf("Ray:    dir     (%f, %f, %f) \n", rayDir.x(), rayDir.y(), rayDir.z());
	std::vector<Entity*> entities = this->entityManager->GetEntities();
	if(this->physServer->RayTrace(r, hits, entities))
	{
		//printf("num hits: %i \n", hits.size());
		for(uint i = 0; i < entities.size(); i++)
		{
			entities[i]->SetSelected(false);
		}
		Collision closestHit = this->physServer->GetClosestCollision(r.startPoint, hits);
		closestHit.entity->SetSelected(true);
		matrix44 mat = matrix44::transMat(closestHit.hitPoint);
		this->ballHit->SetTransformMatrix(mat);
		vector4 direction = r.direction;

		Force f;
		f.CalculateForce(direction, dir * 0.1f);
		f.hitPoint = closestHit.hitPoint;		
		this->physServer->ApplyForceToEntity(closestHit.entity, f);
		closestHit.entity->CalculateInertiaTensorFromOBB();
	}
}

//------------------------------------------------------------------------------
/**
*/
void PhysicsProject::LoadWorld()
{
	// cube
	matrix44 rot_x = matrix44::rotation_x(45.0f, true);
	matrix44 rot_y = matrix44::rotation_y(45.0f, true);
	CreateCube(50.0f, vector4(0.0f, 0.0f, 0.0f), rot_x * rot_y, "first");
	CreateCube(50.0f, vector4(0.0f, 0.0f, 1.5f), rot_x, "second");

	// quad
	//CreateQuad(5.0f, vector4(0.0f, 0.0f, -3.0f));

	// ray balls
	CreateRayBalls();
}

void PhysicsProject::CreateCube(float mass, vector4& pos, matrix44& rot, std::string name)
{
	matrix44 transMat;

	Entity* ent = new Entity();
	ent->name = name;
	
	ent->SetMass(mass);

	MeshResource* res = new MeshResource("cubetri.obj");
	ent->SetMesh(res);

	ShaderObject* shader = new ShaderObject("RastBasicShader.vert", "RastBasicShader.frag");
	ent->SetShader(shader);

	TextureResource* tex = new TextureResource("WORK.png");
	ent->SetTex(tex);

	transMat = matrix44::transMat(pos);

	ent->SetTransformMatrix(transMat);
	ent->SetRotationMatrix(rot);
	ent->SetBoundingBoxes();

	this->entityManager->AddEntity(ent);
}

void PhysicsProject::CreateQuad(float mass, vector4& pos)
{
	matrix44 rot;
	matrix44 transMat;

	Entity* ent = new Entity();
	ent->SetMass(mass);

	MeshResource* res = new MeshResource("quad.obj");
	ent->SetMesh(res);

	ShaderObject* shader = new ShaderObject("RastBasicShader.vert", "RastBasicShader.frag");
	ent->SetShader(shader);

	TextureResource* tex = new TextureResource("WORK.png");
	ent->SetTex(tex);

	transMat = matrix44::transMat(pos);
	rot = matrix44();
	ent->SetTransformMatrix(transMat);
	ent->SetRotationMatrix(rot);
	ent->SetBoundingBoxes();
	this->entityManager->AddEntity(ent);
}

void PhysicsProject::CreateRayBalls()
{
	Entity* ent;
	MeshResource* res;
	ShaderObject* shader;
	TextureResource* tex;

	res = new MeshResource("lilsphere.obj");
	shader = new ShaderObject("RastBasicShader.vert", "RastBasicShader.frag");
	tex = new TextureResource("WORK.png");

	// ray hit ball
	ent = new Entity();
	ent->SetMesh(res);
	ent->SetShader(shader);	
	ent->SetTex(tex);
	this->ballHit = ent;

	// ray start ball
	ent = new Entity();
	tex = new TextureResource("red.png");
	ent->SetMesh(res);
	ent->SetShader(shader);
	ent->SetTex(tex);
	this->ballStart = ent;
}


//------------------------------------------------------------------------------
/**
*/
void PhysicsProject::Run()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	double updateStart = glfwGetTime();
	double updateEnd = updateStart;
	double deltaTime = updateEnd - updateStart;
	double fps;
	while (this->window->IsOpen())
	{		
		updateStart = glfwGetTime();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		this->window->Update();

		this->camera->CalculateViewTransformMatrix();
		std::vector<Entity*> entities = this->entityManager->GetEntities();
		
		this->UpdateEntities(deltaTime);

		this->DrawCoordinateSystem();
		this->DrawRay();

		this->DrawAABBs();

		this->DrawEntities();			
		
		this->ballHit->GetTex()->Bind();
		this->ballHit->SetupShader(this->camera);
		this->ballHit->Draw();

		this->ballStart->GetTex()->Bind();
		this->ballStart->SetupShader(this->camera);
		this->ballStart->Draw();


		this->window->SwapBuffers();
		updateEnd = glfwGetTime();
		deltaTime = updateEnd - updateStart;
		fps = 1.0f / deltaTime;
		//printf("Deltatime: %f \n", float(deltaTime));
		//printf("FPS: %f \n", float(fps));
	}
	
}

//------------------------------------------------------------------------------
/**
*/
void
PhysicsProject::RenderUI()
{
#ifdef DEBUG
	char* cstr;
	char* csStr;
	std::string sStr;

	char mass[555];
	char colPoints[555];
	char vel[555];
#endif
	if (this->window->IsOpen())
	{
		bool show = true;

		// create a new window
		ImGui::Begin("Information Table", &show, ImGuiWindowFlags_NoSavedSettings);

		ImGui::SetWindowSize(ImVec2(350.0f, 440.0f), 0);
		//sprintf("What exactly is this?", "I've got no fucking clue");
		/*
		sprintf(this->linear, "Linear: %f %f %f \n", this->bodies[this->object].physicsObject.linearMomentum.x(), this->bodies[this->object].physicsObject.linearMomentum.y(), this->bodies[this->object].physicsObject.linearMomentum.z());
		sprintf(this->angular, "Angular: %f %f %f \n", this->bodies[this->object].physicsObject.angularMomentum.x(), this->bodies[this->object].physicsObject.angularMomentum.y(), this->bodies[this->object].physicsObject.angularMomentum.z());

		sprintf(this->rotation, "Transform: %f %f %f %f\n           %f %f %f %f\n           %f %f %f %f\n           %f %f %f %f\n", this->bodies[this->object].graphicsnode.transform[0], this->bodies[this->object].graphicsnode.transform[1], this->bodies[this->object].graphicsnode.transform[2], this->bodies[this->object].graphicsnode.transform[3],
			this->bodies[this->object].graphicsnode.transform[4], this->bodies[this->object].graphicsnode.transform[5], this->bodies[this->object].graphicsnode.transform[6], this->bodies[this->object].graphicsnode.transform[7],
			this->bodies[this->object].graphicsnode.transform[8], this->bodies[this->object].graphicsnode.transform[9], this->bodies[this->object].graphicsnode.transform[10], this->bodies[this->object].graphicsnode.transform[11],
			this->bodies[this->object].graphicsnode.transform[12], this->bodies[this->object].graphicsnode.transform[13], this->bodies[this->object].graphicsnode.transform[14], this->bodies[this->object].graphicsnode.transform[15]);

		sprintf(this->com, "Center of Mass: %f %f %f \n", this->bodies[this->object].physicsObject.centerofmass.x(),
			this->bodies[this->object].physicsObject.centerofmass.y(),
			this->bodies[this->object].physicsObject.centerofmass.z());

		sprintf(vel, "Velocity: %f %f %f \n", this->bodies[this->object].currentVelocity.x(),
			this->bodies[this->object].currentVelocity.y(),
			this->bodies[this->object].currentVelocity.z());

		sprintf(mass, "Mass: %f \n", this->bodies[this->object].mass);

		/// Gets the name for printing of the Object
		sStr = this->bodies[this->object].name;
		csStr = new char[sStr.length()];
		strcpy(csStr, sStr.c_str());

		ImGui::Text(csStr);

		ImGui::Text(this->angular);
		ImGui::Text(this->linear);
		ImGui::Text(this->rotation);
		ImGui::Text(this->com);
		ImGui::Text(vel);
		ImGui::Text(mass);

		/// Gets the name for printing that is the closest to hit something
		std::string str = this->col.bodies[0]->name;
		cstr = new char[str.length()];
		strcpy(cstr, str.c_str());

		ImGui::Text("--------------------------------");
		sprintf(this->normal, "Normal: %f %f %f \n", this->ray.normal.x(), this->ray.normal.y(), this->ray.normal.z());
		sprintf(this->hit, "Point: %f %f %f \n", this->ray.hit.x(), this->ray.hit.y(), this->ray.hit.z());

		ImGui::Text(this->normal);
		ImGui::Text(this->hit);
		ImGui::Text("--------------------------------");
		ImGui::Text(cstr);
		sprintf(colPoints, "Collision Size: %i \n", this->col.colpoints.size());
		ImGui::Text(colPoints);
		ImGui::Text("--------------------------------");

		/*if (this->bodies[2].aabb.indicator == INSIDE)
			ImGui::Text("Inside of Box 2");
		if (this->bodies[3].aabb.indicator == INSIDE)
			ImGui::Text("Inside of Box 3");
		if (this->bodies[4].aabb.indicator == INSIDE)
			ImGui::Text("Inside of Box 4");
		
		*/

		ImGui::Text("Movement: WASD");
		ImGui::Text("Rotate Camera: Middle mouse + drag");
		ImGui::Text("Move Camera Forwards and Backwards: Scrollwheel");
		ImGui::Text("Apply Force: LMB");
		ImGui::Text("--------------------------------");
		ImGui::Text("Test cases");
		ImGui::Text("1: F1");
		ImGui::Text("--------------------------------");
		ImGui::Text("Grid");
		ImGui::Text("+X - Red");
		ImGui::Text("-X - Purple");
		ImGui::Text("+Y - Green");
		ImGui::Text("-Y - Yellow");
		ImGui::Text("+Z - Blue");
		ImGui::Text("-Z - Turqoise");
		ImGui::Text("--------------------------------");
		ImGui::Text("Debug data");
		ImGui::Text("Camera pos: (%f, %f, %f)", this->camera->GetPos().x(), this->camera->GetPos().y(), this->camera->GetPos().z());
		ImGui::Text("AABBs collide: %s", this->collisionTest->aabbsCollide ? "yes" : "no");
				
		/*
				// create text editors for shader code
				ImGui::InputTextMultiline("Vertex Shader", vsBuffer, STRING_BUFFER_SIZE, ImVec2(-1.0f, ImGui::GetTextLineHeight() * 16),
					ImGuiInputTextFlags_AllowTabInput);

				ImGui::InputTextMultiline("Pixel Shader", fsBuffer, STRING_BUFFER_SIZE, ImVec2(-1.0f, ImGui::GetTextLineHeight() * 16),
					ImGuiInputTextFlags_AllowTabInput);

				// apply button
				if (ImGui::Button("Apply"))
				{
					// if pressed we compile the shaders
					this->CompileShaders();
				}
				if (this->compilerLog.length())
				{
					// if compilation produced any output we display it here
					ImGui::TextWrapped(this->compilerLog.c_str());
				}
				*/
				// close window
		ImGui::End();
	}
}

//------------------------------------------------------------------------------
/**
*/
void
PhysicsProject::RenderNano(NVGcontext* vg)
{
	nvgSave(vg);

	nvgBeginPath(vg);
	//	nvgCircle(vg,600, 100, 50);
	NVGpaint paint;
	paint = nvgLinearGradient(vg, 600, 100, 650, 150, nvgRGBA(255, 0, 0, 255), nvgRGBA(0, 255, 0, 255));
	nvgFillPaint(vg, paint);
	nvgFill(vg);

	nvgRestore(vg);
}
