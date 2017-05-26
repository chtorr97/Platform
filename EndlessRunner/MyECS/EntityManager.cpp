#include "EntityManager.h"

EntityManager::EntityManager()
{
	createEntity();
}

EntityManager::~EntityManager()
{
}

int EntityManager::createEntity()
{
	int entityIndex = -1;
	if (!freeIndex.empty())
	{
		entityIndex = popPool();
	}
	else
	{
		entities.emplace_back(CM);
		entityIndex = entities.size() - 1;
	}
	return entityIndex;
}

void EntityManager::deleteEntity(int ID)
{
	if (ID < entities.size())
	{
		entities.at(ID).dispose();
		freeIndex.insert(ID);
	}
}

void EntityManager::deleteEntity(iterator it)
{
	deleteEntity(it - entities.begin());
}

Entity& EntityManager::get(int ID)
{
	if (ID < entities.size())
		return entities.at(ID);
	return entities.at(0);
}

int EntityManager::popPool()
{
	int tmp = *freeIndex.begin();
	freeIndex.erase(freeIndex.begin());
	return tmp;
}