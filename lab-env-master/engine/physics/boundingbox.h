#pragma once
#include <math/vector4.h>
#include <vector>
#include <math/matrix44.h>
#include <render/meshresource.h>

namespace Math {
struct Ray;
}

using namespace Math;
namespace Physics
{

enum BoundingType
{
	AABB,
	OBB
};

class ALIGN16 BoundingBox
{
public:

	BoundingBox();
	~BoundingBox();
	void DefineBoundsByMesh(Render::MeshResource* mesh);
	void DefineBoundsByBox(BoundingBox& box);
	void SetMinMaxPoints();
	void SetCenterPoint();
	void SetBoundingType(BoundingType type);
	BoundingType GetBoundingType();
		
	void MultiplyBoundsWithMatrix(matrix44& mat);
	vector4 GetBoxDimensions();
	bool BoxIntersectsRay(Ray & ray);

	float max_x, min_x;
	float max_y, min_y;
	float max_z, min_z;
	float foo1, foo2; // Extra fluff to make it 16 aligned for SSE
	vector4 bounds[8];
	vector4 max_point;
	vector4 min_point;
	vector4 center;
private:

	BoundingType type;

	
};

inline void BoundingBox::SetBoundingType(BoundingType type)
{
	this->type = type;
}

inline BoundingType BoundingBox::GetBoundingType()
{
	return this->type;
}
}