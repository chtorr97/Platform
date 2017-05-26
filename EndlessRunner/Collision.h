#ifndef COLLISION_H
#define COLLISION_H

#include "MyECS\EntityManager.h"

namespace collision
{
	inline void kinematic(EntityManager::iterator actor, EntityManager::iterator platform, Vector2f delta, Vector2f intersection)
	{
		Vector2f dPos = actor->get<comp::transform>().getDeltaPos();

		if (abs(dPos.x) > abs(dPos.y))
		{
			if (delta.x > 0.0f)
			{
				actor->get<comp::transform>().move(intersection.x, 0.0f);
			}
			else
			{
				actor->get<comp::transform>().move(-intersection.x, 0.0f);
			}
		}
		else
		{
			if (delta.y > 0.0f)
			{
				actor->get<comp::transform>().move(0.0f, intersection.y);
				actor->get<comp::player>().touchGround = true;
			}
			else
			{
				actor->get<comp::transform>().move(0.0f, -intersection.y);
			}
		}
		actor->get<comp::collider>().setPosition(actor->get<comp::transform>().position);
		/*
				if (intersection.x < intersection.y)
				{
					if (delta.y > 0.0f)
					{
						actor->get<comp::transform>().move(0.0f, intersection.y * (1.0f - push));
						platform->get<comp::transform>().move(0.0f, -intersection.y * push);
					}
					else
					{
						actor->get<comp::transform>().move(0.0f, -intersection.y * (1.0f - push));
						platform->get<comp::transform>().move(0.0f, intersection.y * push);
					}
					if (actor->has<comp::player>())
						actor->get<comp::player>().touchGround = true;

					if (platform->has<comp::player>())
						platform->get<comp::player>().touchGround = true;
				}
				else
				{
					if (delta.x > 0.0f)
					{
						actor->get<comp::transform>().move(intersection.x * (1.0f - push), 0.0f);
						platform->get<comp::transform>().move(-intersection.x * push, 0.0f);
					}
					else
					{
						actor->get<comp::transform>().move(-intersection.x * (1.0f - push), 0.0f);
						platform->get<comp::transform>().move(intersection.x * push, 0.0f);
					}
				}
				actor->get<comp::collider>().setPosition(actor->get<comp::transform>().position);
				platform->get<comp::collider>().setPosition(platform->get<comp::transform>().position);
			}*/
	}

	inline void match(EntityManager& em, EntityManager::iterator e1, EntityManager::iterator e2, Vector2f delta, Vector2f intersection)
	{
		if (e2->getTag() == "player")
		{
			match(em, e2, e1, delta, intersection);
			return;
		}

		if (e1->getTag() == "player")
		{
			if (e2->getTag() == "platform")
				kinematic(e1, e2, delta, intersection);
			else if (e2->getTag() == "fire")
				e1->get<comp::player>().isAlive = false;
			else if (e2->getTag() == "coin")
			{
				e1->get<comp::player>().points += 25;
				em.deleteEntity(e2);
			}
		}
		
	}
}

#endif