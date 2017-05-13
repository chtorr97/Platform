#ifndef ENTITY_H
#define ENTITY_H

#include <memory>
#include <iostream>
#include <bitset>
#include <unordered_map>

#include "ComponentsManager.h"

using comp::comps;

class Entity
{
public:
	Entity();
	Entity(ComponentsManager& _CM);
	~Entity();

	void setComponentManager(ComponentsManager& _CM) { CM = &_CM; }

	bitset<comps::NUMBER_OF_COMPONENTS> getMask() { return mask; }

	bool isActive() { return mask[comp::IS_ACTIVE]; }
	void activate() { mask |= comp::maskOf(comp::IS_ACTIVE); };
	void deactivate() { mask = mask & ~comp::maskOf(comp::IS_ACTIVE); };

	template<class T>
	bool has()
	{
		return componentIndex.find(T::TYPE) != componentIndex.end();
	}

	template<class T>
	bool add()
	{
		if (has<T>())
			return false;
		componentIndex[T::TYPE] = CM->add<T>();
		mask |= comp::maskOf(T::TYPE);
		return true;
	}

	template<class T>
	void remove()
	{
		if (has<T>())
		{
			CM->remove(componentIndex[T::TYPE]);
			componentIndex.erase(T::TYPE);
			mask = mask & ~comp::maskOf(T::TYPE);
		}
		else
			cerr << "ID doesn't have component " << T::TYPE << endl;
	}

	template<class T>
	T& get()
	{
		if (has<T>())
		{
			return CM->get<T>(componentIndex[T::TYPE]);
		}
		cerr << "ID doesnt have component " << T::TYPE << endl;
		T a;
		return a;
	}

	void dispose()
	{
		mask = comp::maskOf(comps::NONE);

		for (int k = 0; k < componentIndex.size(); k++)
		{
			if (componentIndex.find(k) != componentIndex.end())
			{
				CM->remove(componentIndex[k]);
			}
		}
		componentIndex.clear();
	}

private:
	bitset<comps::NUMBER_OF_COMPONENTS> mask;
	unordered_map<int, int> componentIndex;
	ComponentsManager* CM;
};

#endif
