#include "Entity.h"

Entity::Entity()
	: mask(0)
{
}

Entity::Entity(ComponentsManager& _CM)
	: CM(&_CM), mask(0)
{
}

Entity::~Entity()
{
	//destroy();
	//cout << "~" << endl;
}