#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "Entity.h"
#include "ComponentsManager.h"

class EntityManager
{
	using entities_t = std::vector<Entity>;
public:
	using iterator = entities_t::iterator;
	using const_iterator = entities_t::const_iterator;

	EntityManager();
	~EntityManager();

	int createEntity();

	void deleteEntity(int ID);

	Entity& get(int ID);

	inline const int getSize() { return entities.size(); }

	iterator begin() { return entities.begin(); }
	iterator end() { return entities.end(); }
	const_iterator begin() const { return entities.begin(); }
	const_iterator end() const { return entities.end(); }
	const_iterator cbegin() const { return entities.cbegin(); }
	const_iterator cend() const { return entities.cend(); }

private:
	entities_t entities;
	ComponentsManager CM;

	set<int> freeIndex;

	int popPool();
};

#endif