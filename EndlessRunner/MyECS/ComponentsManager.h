#ifndef COMPONENTS_MANAGER_H
#define COMPONENTS_MANAGER_H

#include <vector>
#include <set>
#include <iostream>

#include "Components.h"

using namespace std;

class ComponentsManager
{
public:
	ComponentsManager();
	~ComponentsManager();

	template<class T>
	T& get(int index)
	{
		return components.at(index).get<T>();
	}

	template<class T>
	int add()
	{
		int componentIndex = -1;
		if (!freeIndex.empty())
		{
			componentIndex = popPool();
		}
		else
		{
			components.push_back(unionType());
			componentIndex = components.size() - 1;
		}
		components.at(componentIndex).element = T();
		components.at(componentIndex).get<T>().onCreate();
		return componentIndex;
	}

	void remove(int index)
	{
		freeIndex.insert(index);
	}

private:

	struct unionType
	{
		comp::compVar element;

		template<class T>
		T& get()
		{
			return boost::get<T>(element);
		}
	};

	vector<unionType> components;
	set<int> freeIndex;

	int popPool();
};

#endif