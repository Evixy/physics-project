#pragma once
#include "entity.h"

namespace System
{
class EntityManager
{
public:
	
	~EntityManager();

	static EntityManager* Instance();

	Entity * RemoveEntity(Entity* ent, bool deleteEntity = false);

	void AddEntity(Entity* ent);
	
	Entity* GetEntityByName(std::string& entityName);
	Entity* GetEntityByName(const char* entityName);
	std::vector<Entity*>& GetEntities();

private:
	EntityManager();
	std::vector<Entity*> entities;
};

	
}
