#pragma once
#include <vector>
#include "math/vector4.h"
#include "math/simplex.h"
#include "system/entity.h"

namespace System {
class Entity;
}

namespace Physics
{
class CollisionTest
{
public:
	static CollisionTest* Instance();
	void CheckCollision(std::vector<System::Entity*> entities);
	bool aabbsCollide = true;

private:

	CollisionTest();

	bool CheckBroadCollision(System::Entity* entity1, System::Entity* entity2);
	Math::vector4 Support(System::Entity* entity1, System::Entity* entity2, Math::vector4& dir);
	bool TestGJKCollision(System::Entity* entity1, System::Entity* entity2);
	bool ContainsOrigin(Math::simplex& simp, Math::vector4& dir);
	bool Tetrahedron(Math::simplex& s, Math::vector4& dir);
	bool Triangle(Math::simplex& s, Math::vector4& dir);
	bool Line(Math::simplex& s, Math::vector4& dir);
	bool VectorPointsToOrigin(Math::vector4& vec, Math::vector4& origin);
	static bool SortByAabbX(const System::Entity* ent1, const System::Entity* ent2)
	{
		return ent1->aabb.min_x < ent2->aabb.min_x;
	}

	
};
}
