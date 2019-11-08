#include "collision.h"
#include <math/miscmath.h>
#include <math/plane.h>
#include <system/entity.h>

using namespace System;

namespace Physics
{
	class Physics
	{
		static bool RayTrace(Ray& ray, std::vector<Collision>& hits, std::vector<Entity*>& worldObjects);
	};
bool RayTrace(Ray& ray, std::vector<Collision>& hits, std::vector<Entity*>& worldObjects);

inline bool RayTrace(Ray& ray, std::vector<Collision>& hits, std::vector<Entity*>& worldObjects)
{
	for (uint i = 0; i < worldObjects.size(); i++)
	{
		Entity* ent = worldObjects[i];
		BoundingBox box = ent->GetAABB();
		assert(box.GetBoundingType() == AABB);
		if (box.BoxIntersectsRay(ray))
		{
			for (uint j = 0; j < ent->GetMesh()->vb.size(); j += 3)
			{
				vector4 v1 = ent->GetMesh()->vb[j].pos;
				vector4 v2 = ent->GetMesh()->vb[j + 1].pos;
				vector4 v3 = ent->GetMesh()->vb[j + 2].pos;

				plane p = plane(v1, v2, v3);

				vector4 isp;
				if (ray.Intersection(p, isp))
				{
					isp.set_w(0.0f);
					if (PointInTriangle(isp, v1, v2, v3))
					{
						Collision c;
						c.hitPoint = isp;
						c.mesh = ent->GetMesh();
						hits.push_back(c);
					}
					else
					{
						printf("Point not in triangle - isp(%f, %f, %f) \n", isp.x(), isp.y(), isp.z());
					}
				}
				else
				{
					printf("No intersection with the plane \n");
				}
			}
		}
		else
		{
			printf("Failed to intersect a box \n");
		}
	}

	if (hits.size() > 0)
	{
		return true;
	}
	return false;
}

}
