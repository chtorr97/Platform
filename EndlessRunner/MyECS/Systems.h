#ifndef SYSTEMS_H
#define SYSTEMS_H

#include "Utility.h"
#include "Collision.h"

namespace sys
{
	static bitset<comps::NUMBER_OF_COMPONENTS> maskGravity(maskOf(comps::IS_ACTIVE) | maskOf(comps::TRANSFORM) | maskOf(comps::PHYSIC));
	void gravity(EntityManager& em, float dt)
	{
		for (EntityManager::iterator e = em.begin(); e < em.end(); e++)
		{
			if ((e->getMask() & maskGravity) == maskGravity && !e->get<comp::physic>().isKinematic)
			{
				e->get<comp::physic>().applyForce(0, 0.16 * dt);
			}
		}
	}

	static bitset<comps::NUMBER_OF_COMPONENTS> maskMovement(maskOf(comps::IS_ACTIVE) | maskOf(comps::TRANSFORM) | maskOf(comps::PHYSIC));
	void movement(EntityManager& em, float dt = 1)
	{
		for (EntityManager::iterator e = em.begin(); e < em.end(); e++)
		{
			if ((e->getMask() & maskMovement) == maskMovement)
			{
				e->get<comp::physic>().update();
				if (abs(e->get<comp::physic>().velocity.x) > e->get<comp::physic>().maxSpeed)
				{
					e->get<comp::physic>().velocity.x /= abs(e->get<comp::physic>().velocity.x);
					e->get<comp::physic>().velocity.x *= e->get<comp::physic>().maxSpeed;
				}
				e->get<comp::transform>().move(e->get<comp::physic>().velocity * dt);

				if (e->has<comp::collider>())
				{
					e->get<comp::collider>().setPosition(e->get<comp::transform>().position);
				}

				e->get<comp::physic>().reset();
			}
		}
	}

	static bitset<comps::NUMBER_OF_COMPONENTS> maskCollision(maskOf(comps::IS_ACTIVE) | maskOf(comps::COLLIDER));
	void collision(EntityManager& em)
	{
		for (EntityManager::iterator e = em.begin(); e < em.end(); e++)
		{
			if ((e->getMask() & maskCollision) == maskCollision)
			{
				for (EntityManager::iterator other = next(e, 1); other < em.end(); other++)
				{
					if ((other->getMask() & maskCollision) == maskCollision)
					{ //make it so that it calls a custom method to resolve / functors? -> if colliding with player then fx / std::function
						Vector2f delta = other->get<comp::collider>().getPosition() - e->get<comp::collider>().getPosition();
						Vector2f intersection = Vector2f(
							abs(delta.x) - (e->get<comp::collider>().getHalfSize().x + other->get<comp::collider>().getHalfSize().x),
							abs(delta.y) - (e->get<comp::collider>().getHalfSize().y + other->get<comp::collider>().getHalfSize().y));

						if (intersection.x < 0.0f && intersection.y < 0.0f) // there's a collision
						{
							collision::match(em, e, other, delta, intersection);
						}

					}
				}
			}
		}
	}

	static bitset<comps::NUMBER_OF_COMPONENTS> maskDisplay(maskOf(comps::IS_ACTIVE) | maskOf(comps::TRANSFORM) | maskOf(comps::GRAPHIC));
	void display(EntityManager& em, RenderWindow& window, float dt = 1) //if lag -> draw only in sight
	{
		for (EntityManager::iterator e = em.begin(); e < em.end(); e++)
		{
			IntRect tmp;
			if ((e->getMask() & maskDisplay) == maskDisplay)
			{
				e->get<comp::graphic>().sprite.setPosition(e->get<comp::transform>().position);
				e->get<comp::graphic>().sprite.setRotation(e->get<comp::transform>().heading);

				if (e->has<comp::animation>())
				{
					tmp = e->get<comp::graphic>().texLocation;
					e->get<comp::animation>().update(dt);
					tmp.left += tmp.width * e->get<comp::animation>().getFrame();
					tmp.top += tmp.height * e->get<comp::animation>().getAnimation();
					e->get<comp::graphic>().sprite.setTextureRect(tmp);
				}
				else
				{
					e->get<comp::graphic>().sprite.setTextureRect(e->get<comp::graphic>().texLocation);
				}

				window.draw(e->get<comp::graphic>().sprite);
			}
		}
	}

	void handleInput(EntityManager& em, int player, Vector2f cursor)
	{
		Entity& e = em.get(player);
		if (Keyboard::isKeyPressed(Keyboard::Space) && e.get<comp::player>().touchGround && e.get<comp::player>().canJump)
		{
			e.get<comp::physic>().applyForce({ 0, -48 });
			e.get<comp::player>().canJump = false;
		}
		else if (!Keyboard::isKeyPressed(Keyboard::Space))
		{
			e.get<comp::player>().canJump = true;
		}
		/*if (Keyboard::isKeyPressed(Keyboard::A))
			e.get<comp::physic>().applyForce({ -1, 0 });
		if (Keyboard::isKeyPressed(Keyboard::D))*/
		if (e.get<comp::player>().isAlive)
			e.get<comp::physic>().applyForce({ 1, 0 });/*
		if (Keyboard::isKeyPressed(Keyboard::S))
			e.get<comp::physic>().applyForce({ 0, 1 });*/

		e.get<comp::player>().touchGround = false;
		//e.get<comp::transform>().heading = 180 - atan2(cursor.x - e.get<comp::transform>().position.x, cursor.y - e.get<comp::transform>().position.y) * ut::TO_DEGREE;
	}

	void cleanFarFromPlayer(EntityManager& em, int player)
	{
		for (EntityManager::iterator e = em.begin(); e < em.end(); e++)
		{
			if ((e->getMask() & ((maskOf(comps::IS_ACTIVE) | maskOf(comps::TRANSFORM)))) == ((maskOf(comps::IS_ACTIVE) | maskOf(comps::TRANSFORM))))
			{
				if (e->get<comp::transform>().position.x < em.get(player).get<comp::transform>().position.x - 1500)
					em.deleteEntity(e);
			}
		}
	}

	void cleanAll(EntityManager& em)
	{
		for (EntityManager::iterator e = em.begin(); e < em.end(); e++)
		{
			em.deleteEntity(e);
		}
	}

}

#endif