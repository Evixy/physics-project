#include "config.h"
#include "camera.h"

Camera::Camera(int screenWidth, int screenHeight):
	xRot(0.0f),
	yRot(0.0f),
	zRot(0.0f),
	viewMatrixDirty(true)
{
	this->screenWidth = float(screenWidth);
	this->screenHeight = float(screenHeight);
	this->aspectRatio = this->screenWidth / this->screenHeight;
	this->nearZ = 0.1f;
	this->farZ = 1000.0f;
	this->fov = 100.0f;
	this->projMatrix = matrix44::genProjMatrix(this->fov, this->aspectRatio, this->nearZ, this->farZ);
	//this->TranslateCamera(vector4(start_x, start_y, start_z));
}

Camera::Camera(int screenWidth, int screenHeight, float xPos, float yPos, float zPos) :
	xRot(0.0f),
	yRot(0.0f),
	zRot(0.0f),
	viewMatrixDirty(true)
{
	this->screenWidth = float(screenWidth);
	this->screenHeight = float(screenHeight);
	this->aspectRatio = this->screenWidth / this->screenHeight;
	this->nearZ = 0.1f;
	this->farZ = 1000.0f;
	this->fov = 100.0f;
	this->projMatrix = matrix44::genProjMatrix(this->fov, this->aspectRatio, this->nearZ, this->farZ);
	this->TranslateCamera(vector4(xPos, yPos, zPos));
}

Camera::~Camera()
{
	//Empty
}

//------------------------------------------------------------------------------
/**
*/
void* Camera::operator new(size_t i)
{
	return _mm_malloc(i, 16);
}

//------------------------------------------------------------------------------
/**
*/
void Camera::operator delete(void* p)
{
	_mm_free(p);
}

//------------------------------------------------------------------------------
/**
*/
void Camera::CalculateViewTransformMatrix()
{
	matrix44 rotX = matrix44::rotation_x(this->xRot);
	matrix44 rotY = matrix44::rotation_y(this->yRot);
	matrix44 rotZ = matrix44::rotation_z(this->zRot);

	matrix44 viewRotMat = rotX * rotY * rotZ;
	//matrix44 viewRotMat = rotZ * rotY * rotX;
	vector4 pos = this->GetPos();
	matrix44 viewTransMat = matrix44::transMatFromVec(pos);

	this->viewTransformMatrix = viewRotMat * viewTransMat;
	//this->viewTransformMatrix = viewTransMat * viewRotMat;
	this->viewMatrixDirty = true;
}

//------------------------------------------------------------------------------
/**
*/
vector2 Camera::NormalizeScreenCoordinates(vector2& pos)
{
	vector2 normalizedPos;
	normalizedPos[0] = 2.0f * pos[0] / float(this->screenWidth) - 1;
	normalizedPos[1] = -2.0f * pos[1] / float(this->screenHeight) + 1;
	return normalizedPos;
}

//------------------------------------------------------------------------------
/**
*/
vector4 Camera::ScreenToWorld(vector2& pos)
{
	vector2 normPos = this->NormalizeScreenCoordinates(pos);
	float x = normPos[0];
	float y = normPos[1];

	float z = this->nearZ;

	vector4 worldPos = vector4(normPos[0], normPos[1], this->nearZ, 1.0f);
	//matrix44 viewTransposed = this->viewMatrix.transpose();
	//matrix44 viewProj = viewTransposed * this->projMatrix;
	matrix44 viewProj = this->viewTransformMatrix * this->projMatrix;
	matrix44 viewProjInverse = viewProj.inverse();	
	worldPos = matrix44::transform(worldPos, viewProj);
	return worldPos;
}

//------------------------------------------------------------------------------
/**
*/
vector2 Camera::WorldToScreen(vector4& pos)
{
	return vector2(0.0f, 0.0f);
}

//------------------------------------------------------------------------------
/**
*/
vector4 Camera::GetPos()
{
	return this->viewTransformMatrix.getPos();
}

//------------------------------------------------------------------------------
/**
*/
void Camera::TranslateCamera(vector4& vec)
{
	vector4 transformedVec = matrix44::transform(vec, this->GetViewMatrix());
	this->viewTransformMatrix.translateMatrix(transformedVec);
}

//------------------------------------------------------------------------------
/**
*/
matrix44 & Camera::GetViewMatrix()
{
	if(this->viewMatrixDirty)
	{
		this->viewMatrix = this->viewTransformMatrix.inverse();
		this->viewMatrixDirty = false;
	}	
	return this->viewMatrix;
}

//------------------------------------------------------------------------------
/**
*/
inline vector4 ScreenToWorld(vector2& pos, matrix44& view, matrix44& proj, int width, int height)
{

	float x = 2.0f * pos[0] / float(width) - 1;
	float y = -2.0f * pos[1] / float(height) + 1;
	float z = 0.0f;
	printf("converted mouse pos: x: %f, y: %f \n", x, y);
	matrix44 viewProj = proj * view;
	matrix44 viewProjInverse = viewProj.inverse();
	matrix44 id = viewProjInverse * viewProj;

	vector4 vec(x, y, z, 0.0f);


	vector4 tempVec = viewProjInverse * vec;


	vector4 worldPos = tempVec;
	printf("projected mouse pos: x: %f, y: %f, z: %f \n", worldPos[0], worldPos[1], worldPos[2]);
	return worldPos;
}
