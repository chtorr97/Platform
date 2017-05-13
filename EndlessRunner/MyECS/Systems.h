#ifndef SYSTEMS_H
#define SYSTEMS_H

#include "Utility.h"
#include "EntityManager.h"

using namespace comp;

namespace sys
{
	static bitset<comps::NUMBER_OF_COMPONENTS> maskGravity(maskOf(comps::IS_ACTIVE) | maskOf(comps::TRANSFORM));
	static void gravity(EntityManager& EM, float dt)
	{
		for (EntityManager::iterator it = EM.begin(); it < EM.end(); it++)
		{
			if ((it->getMask() & maskGravity) == maskGravity)
			{
				it->get<comp::transform>().position.y += 50 * dt;
			}
		}
	}

	static bitset<comps::NUMBER_OF_COMPONENTS> maskMovement(maskOf(comps::IS_ACTIVE) | maskOf(comps::TRANSFORM) | maskOf(comps::PHYSIC));
	static void movement(EntityManager& em, float dt = 1)
	{
		Entity& e = em.get(0);
		for (EntityManager::iterator it = em.begin(); it < em.end(); it++)
		{
			e = *it;
			if ((e.getMask() & maskMovement) == maskMovement)
			{
				if (e.get<comp::physic>().force != Vector2f(0, 0))
				{
					e.get<comp::transform>().move(e.get<physic>().force / e.get<physic>().mass * dt);
					e.get<physic>().reset();
				}
			}
		}
	}

	static bitset<comps::NUMBER_OF_COMPONENTS> maskDisplay(maskOf(comps::IS_ACTIVE) | maskOf(comps::TRANSFORM) | maskOf(comps::GRAPHIC));
	static void display(EntityManager& em, RenderWindow& window, float dt = 1) //if lag -> draw only in sight
	{
		Entity& e = em.get(0);
		for (EntityManager::iterator it = em.begin(); it < em.end(); it++)
		{
			e = *it;
			IntRect tmp;
			if ((e.getMask() & maskDisplay) == maskDisplay)
			{
				e.get<graphic>().sprite.setPosition(e.get<comp::transform>().position);
				e.get<graphic>().sprite.setRotation(e.get<comp::transform>().heading);

				if (e.has<animation>())
				{
					tmp = e.get<graphic>().texLocation;
					e.get<animation>().update(dt);
					tmp.left += tmp.width * e.get<animation>().getFrame();
					tmp.top += tmp.height * e.get<animation>().getAnimation();
					e.get<graphic>().sprite.setTextureRect(tmp);
				}
				else
				{
					e.get<graphic>().sprite.setTextureRect(e.get<graphic>().texLocation);
				}

				window.draw(e.get<graphic>().sprite);
			}
		}
	}

	static void handleInput(EntityManager& em, int player, Vector2f cursor)
	{
		Entity& e = em.get(player);
		if (Keyboard::isKeyPressed(Keyboard::W))
			e.get<physic>().applyForce({ 0, -e.get<physic>().maxSpeed });
		if (Keyboard::isKeyPressed(Keyboard::A))
			e.get<physic>().applyForce({ -e.get<physic>().maxSpeed, 0 });
		if (Keyboard::isKeyPressed(Keyboard::S))
			e.get<physic>().applyForce({ 0, e.get<physic>().maxSpeed });
		if (Keyboard::isKeyPressed(Keyboard::D))
			e.get<physic>().applyForce({ e.get<physic>().maxSpeed, 0 });

		e.get<comp::transform>().heading = 180 - atan2(cursor.x - e.get<comp::transform>().position.x, cursor.y - e.get<comp::transform>().position.y) * ut::TO_DEGREE;
	}

}

#endif