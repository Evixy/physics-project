#include "config.h"

#include "entitymanager.h"

namespace System
{

//------------------------------------------------------------------------------
/**
*/
EntityManager::EntityManager()
{
}

//------------------------------------------------------------------------------
/**
*/
EntityManager::~EntityManager()
{
}

//------------------------------------------------------------------------------
/**
*/
EntityManager* EntityManager::Instance()
{
	static EntityManager inst;
	return &inst;
}

//------------------------------------------------------------------------------
/**
*/
Entity* EntityManager::RemoveEntity(Entity* ent, bool deleteEntity)
{
	for(uint i = 0; i < this->entities.size(); i++)
	{
		if (ent == this->entities[i])
		{
			if(deleteEntity)
			{
				delete this->entities[i];				
				this->entities.erase(this->entities.begin() + i);
				return nullptr;
			}
			else
			{
				Entity* entity;
				entity = this->entities[i];
				this->entities.erase(this->entities.begin() + i);
				return entity;
			}
		}
	}
	return nullptr;
}

//------------------------------------------------------------------------------
/**
*/
void EntityManager::AddEntity(Entity* ent)
{
	this->entities.push_back(ent);
}

//------------------------------------------------------------------------------
/**
*/
Entity* EntityManager::GetEntityByName(std::string& entityName)
{
	for (Entity* entity : this->entities) 
	{
		if (entity->name == entityName) 
		{
			return entity;
		}
	}
	return nullptr;
}

//------------------------------------------------------------------------------
/**
*/
Entity* EntityManager::GetEntityByName(const char* entityName)
{
	std::string name(entityName);
	return GetEntityByName(name);
}

//------------------------------------------------------------------------------
/**
*/
std::vector<Entity*>& EntityManager::GetEntities()
{
	return this->entities;
}
}
