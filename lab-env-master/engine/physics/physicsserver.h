#pragma once

#include <math/ray.h>
#include <vector>
#include "collision.h"
#include <system/entity.h>

namespace Physics
{
struct Force;

class PhysicsServer
	{
	public:
		static PhysicsServer* Instance();
		static bool RayTrace(Ray& ray, std::vector<Collision>& hits, std::vector<System::Entity*>& worldObjects);
		static Collision & GetClosestCollision(vector4& startPoint, std::vector<Collision> & hits);
		static void ApplyForceToEntity(System::Entity* entity, Force& f);
		static void UpdatePhysics(std::vector<System::Entity*> entities, float deltaTime);
		static void UpdateBoundingBoxes(std::vector<System::Entity*>& vector);
		static matrix44 CalculateIntertiaTensor(System::Entity* ent);
		static vector4 CalculateTorque(System::Entity* entity, Force& force);
		static matrix44 CalculateSkewMatrix(vector4& v);
	private:
		PhysicsServer();
		friend class System::Entity;
	};

}
