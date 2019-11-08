#include "AnimationProject.h"
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
#include "animation/Joint.h"
#include "animation/SkeletonParser.h"

#define THREAD_PHYSICS

using namespace Display;
using namespace Math;

//------------------------------------------------------------------------------
/**
*/
AnimationProject::AnimationProject() :
window(nullptr),
rightMouseIsDown(false),
leftMouseIsDown(false),
leftShiftIsDown(false),
middleMouseIsDown(false),
hasRay(false),
drawSkeleton(true)
{		
	
}

//------------------------------------------------------------------------------
/**
*/
AnimationProject::~AnimationProject()
{
	delete this->camera;
}

void AnimationProject::Initialize()
{
	this->physServer = PhysicsServer::Instance();
	this->entityManager = EntityManager::Instance();
	this->lineShader = new ShaderObject("LineShader.vert", "LineShader.frag");
}

//------------------------------------------------------------------------------
/**
*/
void AnimationProject::Start()
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

void AnimationProject::DrawRay()
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

void AnimationProject::DrawAABBs()
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

void AnimationProject::DrawEntities()
{	
	for(uint i = 0; i < this->entities.size(); i++)
	{
		Entity* ent = this->entities[i];
		if(ent->skeleton && this->drawSkeleton)
		{		
			ent->DrawSkeleton();
		}
		ent->GetTex()->Bind();
		ent->SetupShader(this->camera);		
		ent->Draw();
	}
}

void AnimationProject::UpdateAI(double deltaTime)
{
	
	for (uint i = 0; i < this->entities.size(); i++)
	{
		this->entities[i]->UpdateAI(deltaTime);
	}
}

void AnimationProject::UpdateEntities(double deltaTime)
{
	#ifndef THREAD_PHYSICS
	this->UpdatePhysics(deltaTime);
	#endif // !THREAD_IT
	for(uint i = 0; i < this->entities.size(); i++)
	{
		this->entities[i]->Update(deltaTime);
	}
}

//------------------------------------------------------------------------------
/**
*/
bool AnimationProject::Open()
{
	App::Open();

	this->window = new Display::Window;
	this->camera = new Camera(this->window->GetWidth(), this->window->GetHeight());
	if (this->window->Open())
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		return true;
	}
	return false;
}

//------------------------------------------------------------------------------
/**
*/
void AnimationProject::InitializeInput()
{
	this->window->SetKeyPressFunction([this](int32 key, int32, int32 action, int32)
	{
		if (key == GLFW_KEY_ESCAPE)
		{
			this->window->Close();
		}
		if(key == GLFW_KEY_F1 && action == GLFW_PRESS)
		{
			this->drawSkeleton = !this->drawSkeleton;
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
			{		//Move up
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
	});

	this->window->SetMousePressFunction([this](int32 key, int32 action, int32)
	{

		if (key == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS)
		{
			this->leftMouseIsDown = true;
			//here we do ray cast
			vector2 mousePos(this->mousePos_x, this->mousePos_y);
			float dir = 1.0f;
			this->CheckCollission(mousePos, dir);
			
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
			this->CheckCollission(mousePos, dir);
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

void AnimationProject::CheckCollission(vector2& screenPos, float dir)
{	
	Ray r = Ray::CreateRayFromScreenPos(screenPos, this->camera);
	
	this->ray = r;
	this->hasRay = true;
	std::vector<Physics::Collision> hits;
	matrix44 matStart = matrix44::transMat(r.startPoint);
	this->ballStart->SetTransformMatrix(matStart);
	
	printf("\n");
	printf("Ray Start:  (%f, %f, %f) \n", r.startPoint.x(), r.startPoint.y(), r.startPoint.z());
	printf("Ray Dir:    (%f, %f, %f) \n", r.direction.x(), r.direction.y(), r.direction.z());
	//printf("Screen: pos     (%f, %f) \n", rayScreen.x(), rayScreen.y());
	//printf("Eye:    ray     (%f, %f, %f) \n", rayEye.x(), rayEye.y(), rayEye.z());
	////printf("Eye:    transf  (%f, %f, %f) \n", rayTemp.x(), rayTemp.y(), rayTemp.z());
	//printf("Ray:    start   (%f, %f, %f) \n", rayStart.x(), rayStart.y(), rayStart.z());
	//printf("Ray:    dir     (%f, %f, %f) \n", rayDir.x(), rayDir.y(), rayDir.z());
	std::vector<Entity*> entities = this->entityManager->GetEntities();
	if(this->physServer->RayTrace(r, hits, entities))
	{
		printf("num hits: %i \n", hits.size());
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
		f.CalculateForce(direction,  dir * 0.1f);
		f.hitPoint = closestHit.hitPoint;		
		this->physServer->ApplyForceToEntity(closestHit.entity, f);
		closestHit.entity->CalculateInertiaTensorFromOBB();
		
		int i = 0;
	}
}

void AnimationProject::LoadSkeleton()
{
	SkeletonParser parser;
	Entity* unit = new Entity();
	unit->skeleton = parser.LoadSkeletonFromFile("Unit_Footman.constants");
		
	MeshResource* res = new MeshResource("lilsphere.obj");
	unit->jointBall = res;
	//unit->SetMesh(res);

	ShaderObject* shader = new ShaderObject("RastBasicShader.vert", "RastBasicShader.frag");
	unit->SetShader(shader);

	TextureResource* tex = new TextureResource("WORK.png");
	unit->SetTex(tex);
}

//------------------------------------------------------------------------------
/**
*/
void AnimationProject::LoadWorld()
{
	Entity* ent;
	MeshResource* res;
	ShaderObject* shader;
	TextureResource* tex;
	vector4 pos;
	matrix44 transMat;
	matrix44 rot;
	matrix44 yRotMat;
	matrix44 xRotMat;
	matrix44 zRotMat;

	// cube
	ent = new Entity();
	ent->SetMass(50.0f);

	res = new MeshResource("cubetri.obj");	
	ent->SetMesh(res);

	shader = new ShaderObject("RastBasicShader.vert", "RastBasicShader.frag");	
	ent->SetShader(shader);

	tex = new TextureResource("WORK.png");	
	ent->SetTex(tex);

	pos = vector4(0.0f, 0.0f, 1.0f);
	transMat = matrix44::transMat(pos);
	
	
	
	/*yRotMat = matrix44::rotation_y(PI / 2.0f);
	xRotMat = matrix44::rotation_x(PI / 2.0f);
	zRotMat = matrix44::rotation_z(PI / 2.0f);

	rot = xRotMat * yRotMat * zRotMat;*/
	
	ent->SetTransformMatrix(transMat);
	ent->SetRotationMatrix(rot);
	ent->SetBoundingBoxes();
	this->entityManager->AddEntity(ent);

	// quad
	/*ent = new Entity();
	ent->SetMass(10.0f);

	res = new MeshResource("quad.obj");	
	ent->SetMesh(res);

	shader = new ShaderObject("RastBasicShader.vert", "RastBasicShader.frag");	
	ent->SetShader(shader);

	tex = new TextureResource("WORK.png");	
	ent->SetTex(tex);


	pos = vector4(0.0f, 0.0f, -3.0f);
	transMat = matrix44::transMat(pos);
	rot = matrix44();
	ent->SetTransformMatrix(transMat);
	ent->SetRotationMatrix(rot);
	ent->SetBoundingBoxes();
	this->entityManager->AddEntity(ent);*/

	// ray hit ball
	ent = new Entity();

	res = new MeshResource("lilsphere.obj");	
	ent->SetMesh(res);

	shader = new ShaderObject("RastBasicShader.vert", "RastBasicShader.frag");
	ent->SetShader(shader);

	tex = new TextureResource("WORK.png");	
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



#ifdef THREAD_PHYSICS
void PhysicsUpdate(AnimationProject* gfxNode, Display::Window* window)
{
	double timeStepMs = (1000.0f / 30.0f);

	double updateStart;
	double updateEnd;
	double timeSpent;
	double timeToSleep;
	
	while (window->IsOpen())
	{		
		updateStart = glfwGetTime() * 1000.0f;
		//updateStart = glfwGetTime();
	
		// update physics below
		EntityManager* entManager = EntityManager::Instance();
		PhysicsServer* physServer = PhysicsServer::Instance();
		std::vector<Entity*> entities = entManager->GetEntities();

		double deltaTime = timeStepMs * 0.001f;

		physServer->UpdatePhysics(entities, float(deltaTime));
		physServer->UpdateBoundingBoxes(entities);

		updateEnd = glfwGetTime() * 1000.0f;
		//updateEnd = glfwGetTime();
		timeSpent = updateEnd - updateStart;
		timeToSleep = timeStepMs - timeSpent;
		

		if (timeToSleep > 0)
		{
			int time = int(timeToSleep * 1000.0f);
			//printf("Time to Sleep: %f \n", float(timeToSleep));
			std::this_thread::sleep_for(std::chrono::microseconds(time));
		}
		else
		{
			printf("Physics update took too long, optimization needed \n");
		}
	}
}
#endif // THREAD_IT


//------------------------------------------------------------------------------
/**
*/
void AnimationProject::Run()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
		
	
	double previous = glfwGetTime();
	
#ifdef THREAD_PHYSICS
	std::thread t(PhysicsUpdate, this, this->window);
	t.detach();
#endif // THREAD_IT

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
		this->entities = this->entityManager->GetEntities();

		this->UpdateEntities(deltaTime);

		this->DrawRay();

		this->DrawAABBs();

		this->DrawEntities();			
		
		/*this->ballHit->GetTex()->Bind();
		this->ballHit->SetupShader(this->camera);
		this->ballHit->Draw();

		this->ballStart->GetTex()->Bind();
		this->ballStart->SetupShader(this->camera);
		this->ballStart->Draw();*/

		this->window->SwapBuffers();
		updateEnd = glfwGetTime();
		deltaTime = updateEnd - updateStart;
		fps = 1.0f / deltaTime;
		//printf("Deltatime: %f \n", float(deltaTime));
		//printf("FPS: %f \n", float(fps));
	}
	
}

void AnimationProject::RotateJoints()
{
	if (this->rotateX)
	{
		matrix44 rotation = matrix44::rotation_x(0.005f);
		this->jointRotationX *= rotation;
	}

	if (this->rotateY)
	{
		matrix44 rotation = matrix44::rotation_y(0.005f);
		this->jointRotationY *= rotation;
	}

	if (this->rotateZ)
	{
		matrix44 rotation = matrix44::rotation_z(0.005f);
		this->jointRotationZ *= rotation;
	}

	Skeleton* tempSkele = new Skeleton(*this->skeleton);
	std::vector<Joint*> tempJoints = this->skeleton->GetJoints();
	for (int i = 0; i < int(this->skeleton->GetJoints().size()); i++)
	{
		Joint* joint = this->skeleton->GetJoints()[i];

		if (joint->parentJoint)
		{
			vector4 color = vector4(1.0f, 1.0f, 1.0f, 1.0f);
			if (joint->selected)
			{
				matrix44 rotMatrix;
				if (this->rotateX)
				{
					rotMatrix *= this->jointRotationX;
				}
				if (this->rotateY)
				{
					rotMatrix *= this->jointRotationY;
				}
				if (this->rotateZ)
				{
					rotMatrix *= this->jointRotationZ;
				}
				if (this->rotateX || this->rotateY || this->rotateZ)
				{
					joint->RotateJoint(rotMatrix);// TranslateChildren(rotMatrix, true);						
				}

				color = vector4(1.0f, 0.0f, 0.0f, 1.0f);
			}
			matrix44 trans = joint->jointMatrix * this->modelMatrix;			
		}
	}
}
