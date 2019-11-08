#include "config.h"
#include "boundingbox.h"
#include <render/meshresource.h>
#include <math/ray.h>
#include <math/miscmath.h>

/*
      7__________6
      /|        /|
     / |       / |
   4/__|______/5 |
    |  |______|__|
    |  /3     |  /2
    | /       | /
    |/________|/
    0          1

*/

using namespace Render;
namespace Physics
{
BoundingBox::BoundingBox():
	max_x(-FLT_MAX), min_x(FLT_MAX),
	max_y(-FLT_MAX), min_y(FLT_MAX),
	max_z(-FLT_MAX), min_z(FLT_MAX)
{
}

BoundingBox::~BoundingBox()
{
}

void BoundingBox::DefineBoundsByMesh(Render::MeshResource* mesh)
{
	vector4 v;
	
	for(uint i = 0; i < mesh->vb.size(); i++)
	{
		Math::vector4 p = mesh->vb[i].pos;
		
		if(this->max_x < p.x())
		{
			this->max_x = p.x();
		}
		if(this->min_x > p.x())
		{
			this->min_x = p.x();
		}

		if(this->max_y < p.y())
		{
			this->max_y = p.y();
		}
		if (this->min_y > p.y())
		{
			this->min_y = p.y();
		}

		if (this->max_z < p.z())
		{
			this->max_z = p.z();
		}
		if (this->min_z > p.z())
		{
			this->min_z = p.z();
		}
	}
	//bottom
	this->bounds[0] = vector4(this->min_x, this->min_y, this->min_z);
	this->bounds[1] = vector4(this->max_x, this->min_y, this->min_z);
	this->bounds[2] = vector4(this->max_x, this->min_y, this->max_z);
	this->bounds[3] = vector4(this->min_x, this->min_y, this->max_z);

	//top
	this->bounds[4] = vector4(this->min_x, this->max_y, this->min_z);
	this->bounds[5] = vector4(this->max_x, this->max_y, this->min_z);
	this->bounds[6] = vector4(this->max_x, this->max_y, this->max_z);
	this->bounds[7] = vector4(this->min_x, this->max_y, this->max_z);

	this->SetMinMaxPoints();
	this->SetCenterPoint();
}

void BoundingBox::DefineBoundsByBox(BoundingBox& box)
{
	assert(box.GetBoundingType() == OBB);
	this->max_x = -FLT_MAX; this->min_x = FLT_MAX;
	this->max_y = -FLT_MAX; this->min_y = FLT_MAX;
	this->max_z = -FLT_MAX; this->min_z = FLT_MAX;

	for(uint i = 0; i < 8; i++)
	{
		Math::vector4 p = box.bounds[i];
		if (this->max_x < p.x())
		{
			this->max_x = p.x();
		}
		if (this->min_x > p.x())
		{
			this->min_x = p.x();
		}

		if (this->max_y < p.y())
		{
			this->max_y = p.y();
		}
		if (this->min_y > p.y())
		{
			this->min_y = p.y();
		}

		if (this->max_z < p.z())
		{
			this->max_z = p.z();
		}
		if (this->min_z > p.z())
		{
			this->min_z = p.z();
		}
	}
	//bottom
	this->bounds[0] = vector4(this->min_x, this->min_y, this->min_z);
	this->bounds[1] = vector4(this->max_x, this->min_y, this->min_z);
	this->bounds[2] = vector4(this->max_x, this->min_y, this->max_z);
	this->bounds[3] = vector4(this->min_x, this->min_y, this->max_z);

	//top
	this->bounds[4] = vector4(this->min_x, this->max_y, this->min_z);
	this->bounds[5] = vector4(this->max_x, this->max_y, this->min_z);
	this->bounds[6] = vector4(this->max_x, this->max_y, this->max_z);
	this->bounds[7] = vector4(this->min_x, this->max_y, this->max_z);

	this->SetMinMaxPoints();
	this->SetCenterPoint();
}

void BoundingBox::SetMinMaxPoints()
{
	this->max_point = vector4(this->max_x, this->max_y, this->max_z);
	this->min_point = vector4(this->min_x, this->min_y, this->min_z);
}

void BoundingBox::SetCenterPoint()
{
	this->center.set_x(this->max_x - (this->max_x - this->min_x) * 0.5f);
	this->center.set_y(this->max_y - (this->max_y - this->min_y) * 0.5f);
	this->center.set_z(this->max_z - (this->max_z - this->min_z) * 0.5f);
}

void BoundingBox::MultiplyBoundsWithMatrix(matrix44& mat)
{
	assert(this->type == OBB);
	for (uint i = 0; i < 8; i++)
	{		
		matrix44 transMat = matrix44::transMatFromVec(this->bounds[i]);		
		transMat = transMat * mat;
		this->bounds[i] = transMat.getPos();
	}
}

bool BoundingBox::BoxIntersectsRay(Ray & ray)
{	
	vector4 v1 = this->bounds[0];
	vector4 v2 = this->bounds[1];
	vector4 v3 = this->bounds[2];
	vector4 v4 = this->bounds[3];

	vector4 v5 = this->bounds[4];
	vector4 v6 = this->bounds[5];
	vector4 v7 = this->bounds[6];
	vector4 v8 = this->bounds[7];

	plane p;
	vector4 isp;

	//plane 1
	p = plane(v1, v2, v3);
	if(ray.Intersection(p, isp))
	{
		isp.set_w(0.0f);
		if(PointInQuad(isp, v1, v2, v3, v4))
		{
			return true;
		}
	}
	//plane 2
	p = plane(v5, v6, v7);
	if (ray.Intersection(p, isp))
	{
		isp.set_w(0.0f);
		if (PointInQuad(isp, v5, v6, v7, v8))
		{
			return true;
		}
	}
	//plane 3
	p = plane(v3, v7, v6);
	if (ray.Intersection(p, isp))
	{
		isp.set_w(0.0f);
		if (PointInQuad(isp, v3, v7, v6, v2))
		{
			return true;
		}
	}
	//plane 4
	p = plane(v1, v4, v8);
	if (ray.Intersection(p, isp))
	{
		isp.set_w(0.0f);
		if (PointInQuad(isp, v1, v4, v8, v5))
		{
			return true;
		}
	}
	//plane 5
	p = plane(v1, v5, v6);
	if (ray.Intersection(p, isp))
	{
		isp.set_w(0.0f);
		if (PointInQuad(isp, v1, v5, v6, v2))
		{
			return true;
		}
	}
	//plane 6
	p = plane(v3, v4, v8);
	if (ray.Intersection(p, isp))
	{
		isp.set_w(0.0f);
		if (PointInQuad(isp, v3, v4, v8, v7))
		{
			return true;
		}
	}
	return false;
}

vector4 BoundingBox::GetBoxDimensions()
{
	vector4 vec;
	vector4 vX, vY, vZ;
	vX = this->bounds[1] + this->bounds[0];

	vX = this->bounds[1] - this->bounds[0];
	vY = this->bounds[4] - this->bounds[0];
	vZ = this->bounds[3] - this->bounds[0];
	float x = vX.length3();
	float y = vY.length3();
	float z = vZ.length3();
	vec.set(x, y, z);
	return vec;
}
}
