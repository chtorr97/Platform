#ifndef SYSTEMS_H
#define SYSTEMS_H

#include "Utility.h"
#include "EntityManager.h"

using namespace comp;

namespace sys
{
	static bitset<comps::NUMBER_OF_COMPONENTS> maskGravity(maskOf(comps::IS_ACTIVE) | maskOf(comps::TRANSFORM) | maskOf(comps::PHYSIC));
	static void gravity(EntityManager& em, float dt)
	{
		for (EntityManager::iterator e = em.begin(); e < em.end(); e++)
		{
			if ((e->getMask() & maskGravity) == maskGravity && !e->get<physic>().isKinematic)
			{
				e->get<comp::physic>().applyForce(0, 0.2 * dt);
			}
		}
	}

	static bitset<comps::NUMBER_OF_COMPONENTS> maskMovement(maskOf(comps::IS_ACTIVE) | maskOf(comps::TRANSFORM) | maskOf(comps::PHYSIC));
	static void movement(EntityManager& em, float dt = 1)
	{
		for (EntityManager::iterator e = em.begin(); e < em.end(); e++)
		{
			if ((e->getMask() & maskMovement) == maskMovement)
			{
				if (e->get<comp::physic>().force != Vector2f(0, 0))
				{
					e->get<comp::transform>().move(e->get<physic>().force / e->get<physic>().mass * dt);

					if (e->has<collider>())
					{
						e->get<collider>().setPosition(e->get<comp::transform>().position);
					}

					e->get<physic>().reset();
				}
			}
		}
	}

	static bitset<comps::NUMBER_OF_COMPONENTS> maskCollision(maskOf(comps::IS_ACTIVE) | maskOf(comps::COLLIDER) | maskOf(comps::PHYSIC) | maskOf(comps::TRANSFORM));
	static void collision(EntityManager& em)
	{
		for (EntityManager::iterator e = em.begin(); e < em.end(); e++)
		{
			if ((e->getMask() & maskCollision) == maskCollision)
			{
				for (EntityManager::iterator other = next(e, 1); other < em.end(); other++)
				{
					if ((other->getMask() & maskCollision) == maskCollision)
					{ //make it so that it calls a custom method to resolve / functors? -> if colliding with player then fx / std::function
						float deltaX = other->get<comp::collider>().body.left - e->get<comp::collider>().body.left;
						float deltaY = other->get<comp::collider>().body.top - e->get<comp::collider>().body.top;
						float intersectX = abs(deltaX) - (e->get<comp::collider>().body.width + other->get<comp::collider>().body.width);
						float intersectY = abs(deltaY) - (e->get<comp::collider>().body.height + other->get<comp::collider>().body.height);

						if (intersectX < 0.0f && intersectY < 0.0f) // intersection
						{
							//normal resolution -> use forces
							float push = e->get<physic>().isKinematic ? 1.0f : other->get<physic>().isKinematic ? 0.0f : 0.5f;
							if (intersectX > intersectY)
							{
								if (deltaX > 0.0f)
								{
									e->get<comp::transform>().move(intersectX * (1.0f - push), 0.0f);
									other->get<comp::transform>().move(-intersectX * push, 0.0f);
								}
								else
								{
									e->get<comp::transform>().move(-intersectX * (1.0f - push), 0.0f);
									other->get<comp::transform>().move(intersectX * push, 0.0f);
								}
							}
							else
							{
								if (deltaY > 0.0f)
								{
									e->get<comp::transform>().move(0.0f, intersectY * (1.0f - push));
									other->get<comp::transform>().move(0.0f, -intersectY * push);
								}
								else
								{
									e->get<comp::transform>().move(0.0f, -intersectY * (1.0f - push));
									other->get<comp::transform>().move(0.0f, intersectY * push);
								}
							}
							e->get<collider>().setPosition(e->get<comp::transform>().position);
							other->get<collider>().setPosition(other->get<comp::transform>().position);
						}

					}
				}
			}
		}
	}

	static bitset<comps::NUMBER_OF_COMPONENTS> maskDisplay(maskOf(comps::IS_ACTIVE) | maskOf(comps::TRANSFORM) | maskOf(comps::GRAPHIC));
	static void display(EntityManager& em, RenderWindow& window, float dt = 1) //if lag -> draw only in sight
	{
		for (EntityManager::iterator e = em.begin(); e < em.end(); e++)
		{
			IntRect tmp;
			if ((e->getMask() & maskDisplay) == maskDisplay)
			{
				e->get<graphic>().sprite.setPosition(e->get<comp::transform>().position);
				e->get<graphic>().sprite.setRotation(e->get<comp::transform>().heading);

				if (e->has<animation>())
				{
					tmp = e->get<graphic>().texLocation;
					e->get<animation>().update(dt);
					tmp.left += tmp.width * e->get<animation>().getFrame();
					tmp.top += tmp.height * e->get<animation>().getAnimation();
					e->get<graphic>().sprite.setTextureRect(tmp);
				}
				else
				{
					e->get<graphic>().sprite.setTextureRect(e->get<graphic>().texLocation);
				}

				window.draw(e->get<graphic>().sprite);
			}
		}
	}

	static void handleInput(EntityManager& em, int player, Vector2f cursor)
	{
		Entity& e = em.get(player);
		if (Keyboard::isKeyPressed(Keyboard::W))
			e.get<physic>().applyForce({ 0, -e.get<physic>().maxSpeed * 10 });
		if (Keyboard::isKeyPressed(Keyboard::A))
			e.get<physic>().applyForce({ -e.get<physic>().maxSpeed, 0 });
		if (Keyboard::isKeyPressed(Keyboard::S))
			e.get<physic>().applyForce({ 0, e.get<physic>().maxSpeed });
		if (Keyboard::isKeyPressed(Keyboard::D))
			e.get<physic>().applyForce({ e.get<physic>().maxSpeed, 0 });

		//e.get<comp::transform>().heading = 180 - atan2(cursor.x - e.get<comp::transform>().position.x, cursor.y - e.get<comp::transform>().position.y) * ut::TO_DEGREE;
	}

}

#endif