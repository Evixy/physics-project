#include "config.h"
#include "collisiontest.h"
#include "system/entity.h"
#include "math/simplex.h"
#include <algorithm>

namespace Physics
{

CollisionTest::CollisionTest():
	aabbsCollide(true)
{

}

CollisionTest* CollisionTest::Instance()
{
	static CollisionTest inst;
	return &inst;
}

void CollisionTest::CheckCollision(std::vector<System::Entity*> entities)
{
	std::vector<System::Entity*> entitiesCopy = entities;
	std::sort(entitiesCopy.begin(), entitiesCopy.end(), SortByAabbX);
	for (uint i = 0; i < entitiesCopy.size(); i++)
	{
		for (uint j = i + 1; j < entitiesCopy.size(); j++)
		{
			System::Entity* entity1 = entitiesCopy[i];
			System::Entity* entity2 = entitiesCopy[j];
			if (this->CheckBroadCollision(entity1, entity2))
			{
				//printf("Broad collision check returned true \n");
				if (this->TestGJKCollision(entity1, entity2))
				{
					//printf("Collission on mesh for ent %s ent %s \n", entity1->name.c_str(), entity2->name.c_str());
					// Do response here
				}
			}
		}
	}	
}

bool CollisionTest::CheckBroadCollision(System::Entity* entity1, System::Entity* entity2)
{	
	vector4 ent1_min = entity1->GetAABB().min_point;
	vector4 ent1_max = entity1->GetAABB().max_point;

	vector4 ent2_min = entity2->GetAABB().min_point;
	vector4 ent2_max = entity2->GetAABB().max_point;
	
	if (ent1_max == ent2_max && ent1_min == ent2_min)
	{
		printf("AABBs for entity 1 and 2 are at the same coordinates \n");
		return true;
	}
	
	// Testing x-axis
	if (ent1_max.x() > ent2_min.x() && ent2_max.x() > ent1_min.x())
	{
		// Testing y-axis
		if (ent1_max.y() > ent2_min.y() && ent2_max.y() > ent1_min.y())
		{
			// Testing z-axis
			if (ent1_max.z() > ent2_min.z() && ent2_max.z() > ent1_min.z())
			{
				// AABBs collide.
				this->aabbsCollide = true;
				return true;
			}
		}
	}
	// TODO: Returning true for now. 
	this->aabbsCollide = false;
	return false;
}

vector4 CollisionTest::Support(System::Entity* entity1, System::Entity* entity2, vector4& dir)
{
	vector4 s;
	vector4 sup_a = entity1->GetFurthestPointInDir(dir);
	vector4 sup_b = entity2->GetFurthestPointInDir(-dir);
	s = sup_a - sup_b;
	return s;
}

bool CollisionTest::TestGJKCollision(System::Entity* entity1, System::Entity* entity2)
{
	vector4 dir = vector4(1.0f, 1.0f, 1.0f);
	simplex simp;
	// Getting support point for minkowski difference. 
	vector4 c = this->Support(entity1, entity2, dir);
	simp.add(c);
	// Want to point to the direction to the origin. 
	dir = -c;
	
	for (unsigned int tries = 0; tries < 50; tries++)
	{
		simp.add(this->Support(entity1, entity2, dir));
		float val = vector4::dot3(simp.last(), dir);
		if (val < 0.0f)
		{
			// No collision.
			printf("No intersection for ent %s and ent %s %f \n", entity1->name.c_str(), entity2->name.c_str(), vector4::dot3(simp.last(), dir));
			return false;
		}
		if (this->ContainsOrigin(simp, dir))
		{
			// TODO do EPA
			return true;
		}
	}
	printf("No intersection (timeout) ent %s ent %s \n", entity1->name.c_str(), entity2->name.c_str());
	return false;
}

bool CollisionTest::ContainsOrigin(simplex& simp, vector4& dir)
{
	// Line
	if (simp.size() == 2)
	{
		return this->Line(simp, dir);
	}
	// Triangle
	if (simp.size() == 3)
	{
		return this->Triangle(simp, dir);
	}
	// Tetrahedron
	if (simp.size() == 4)
	{
		return this->Tetrahedron(simp, dir);
	}
	return false;
}

bool CollisionTest::Line(simplex& s, vector4& dir)
{
	assert(s.size() == 2);
	vector4 a, b;
	vector4 ao, ab;
	a = s.a();
	b = s.b();

	ao = -a;
	ab = b - a;

	if (this->VectorPointsToOrigin(ab, ao))
	{
		// Origin is enclosed by the vector ab (the vector ab is in the same direction as the origin). 
		// Search direction is set to a vector that is perpendicular to the vector from crossing ab and a0, and the vector ab
		dir = vector4::crossdouble3(ab, ao);
		// Keeping existing points in the simplex
	}
	else
	{
		// We need to continue the search in the direction of the origin.
		// Point b should not be included in the search because the vector ab didn't enclose the origin (the vector ab is not in the same direction as the origin). 
		dir = ao;
		// Removing b from simplex
		s.remove_b();
	}
	return false;
}

bool CollisionTest::Triangle(simplex& s, vector4& dir)
{
	assert(s.size() == 3);
	vector4 a, b, c;
	vector4 ao, ab, ac, abc;
	a = s.a();
	b = s.b();
	c = s.c();

	ao = -a;
	ab = b - a;
	ac = c - a;
	abc = vector4::cross3(ab, ac);

	if (this->VectorPointsToOrigin(vector4::cross3(abc, ac), ao))
	{
		// The vector that is perpendicular to the triangle abc and the vector ac points in the same direction as the origin.
		if (this->VectorPointsToOrigin(ac, ao))
		{
			// The vector ac points in the same direction as the origin. 
			// We want to do the next search with c and a as the points and in the direction of a vector perpendicular to ac and a0, which in turn is perpendicular to a0.
			dir = vector4::crossdouble3(ac, ao);
			// Removing point b from simplex
			s.remove_b();
		}
		else
		{
			if (this->VectorPointsToOrigin(ab, ao))
			{
				dir = vector4::crossdouble3(ab, ao);
				// Removing point c from simplex
				s.remove_c();
			}
			else
			{
				// Setting dir to origin
				dir = ao;
				// Removing point c and b from simplex				
				s.remove_c();
				s.remove_b();
			}
		}
	}
	else
	{
		if (this->VectorPointsToOrigin(vector4::cross3(ab, abc), ao))
		{
			// The vector from ab x abc points to origin. 	
			if (this->VectorPointsToOrigin(ab, ao))
			{
				dir = vector4::crossdouble3(ab, ao);
				// Removing point c from simplex
				s.remove_c();
			}
			else
			{
				// Setting dir to origin
				dir = ao;
				// Removing point c and b from simplex
				s.remove_c();
				s.remove_b();
			}
		}
		else
		{
			// If we get to here we can build a tetrahedron
			// The vector from ab x abc doesn't point to origin. 
			// Checking if origin is above the triangle
			if (this->VectorPointsToOrigin(abc, ao))
			{
				// Vector abc points to origin. Include all points in the same order and search in the direction of abc.
				dir = abc;
				// Keeping the simplex the same. 
			}
			else
			{
				// The vector abc doesn't point to origin. Search in the direction of the origin from abc.
				dir = vector4::zero() - abc;
				// Keep points but switch b and c around to keep the counter-clockwise triangle.
				s.clear();
				s.add(b);
				s.add(c);
				s.add(a);
			}
		}
	}
	return false;
}

bool CollisionTest::Tetrahedron(simplex& s, vector4& dir)
{
	assert(s.size() == 4);
	vector4 a, b, c, d;
	vector4 ao, ab, ac, ad, abc, acd, adb;
	a = s.a();
	b = s.b();
	c = s.c();
	d = s.d();

	ao = -a;
	ab = b - a;
	ac = c - a;
	
	abc = vector4::cross3(ab, ac);
	if (this->VectorPointsToOrigin(abc, ao))
	{
		// Origin is in front of triangle ABC
		// Removing point d from simplex
		s.remove_d();
		return this->ContainsOrigin(s, dir);
	}
	ad = d - a;
	acd = vector4::cross3(ac, ad);
	if (this->VectorPointsToOrigin(acd, ao))
	{
		// Origin is in front of triangle ACD
		// Removing point b from simplex
		s.remove_b();
		return this->ContainsOrigin(s, dir);
	}
	adb = vector4::cross3(ad, ab);
	if (this->VectorPointsToOrigin(adb, ao))
	{
		// Origin is in front of triangle ADB
		// Removing point c from simplex
		s.remove_c();
		return this->ContainsOrigin(s, dir);		
	}
	// Origin is contained within the tetrahedron and therefore we have a collision.
	return true;	
}

bool CollisionTest::VectorPointsToOrigin(Math::vector4& vec, Math::vector4& origin)
{
	float val = vector4::dot3(vec, origin);
	return val > 0.0f;
}
}

