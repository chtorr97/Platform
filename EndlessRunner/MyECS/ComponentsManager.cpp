#include "ComponentsManager.h"

ComponentsManager::ComponentsManager()
{
}

ComponentsManager::~ComponentsManager()
{
}

int ComponentsManager::popPool()
{
	int tmp = *freeIndex.begin();
	freeIndex.erase(freeIndex.begin());
	return tmp;
}