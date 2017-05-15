#ifndef ASSET_H
#define ASSET_H

#include "MyECS\Systems.h"
#include "TextureManager.h"

namespace asset
{
	static int createPlayer(EntityManager& em, TextureManager& tm, Vector2f pos = { 0, 0 }, float mass = 10, Vector2f size = { 32, 32 })
	{
		int newPlayer = em.createEntity();

		em.get(newPlayer).activate();
		em.get(newPlayer).add<comp::player>();
		em.get(newPlayer).add<comp::transform>();
		em.get(newPlayer).add<comp::physic>();
		em.get(newPlayer).add<comp::graphic>();
		em.get(newPlayer).add<comp::collider>();
		em.get(newPlayer).get<comp::transform>().position = pos;
		em.get(newPlayer).get<comp::physic>().mass = mass;
		em.get(newPlayer).get<comp::graphic>().texture = &tm.get("player");
		em.get(newPlayer).get<comp::graphic>().texLocation = IntRect(0, 0, size.x, size.y);
		em.get(newPlayer).get<comp::graphic>().init();
		em.get(newPlayer).get<comp::collider>().setSize(size.x / 2, size.y / 2);
		em.get(newPlayer).get<comp::collider>().setPosition(em.get(newPlayer).get<comp::transform>().position);

		return newPlayer;
	}

	static int createPlatform(EntityManager& em, TextureManager& tm, Vector2f pos = { 0, 0 }, float mass = 10, Vector2f size = { 32, 32 })
	{
		int newPlatform = em.createEntity();

		em.get(newPlatform).activate();
		em.get(newPlatform).add<comp::transform>();
		em.get(newPlatform).add<comp::physic>();
		em.get(newPlatform).add<comp::graphic>();
		em.get(newPlatform).add<comp::collider>();
		em.get(newPlatform).get<comp::transform>().position = pos;
		em.get(newPlatform).get<comp::physic>().mass = 10;
		em.get(newPlatform).get<comp::physic>().isKinematic = true;
		em.get(newPlatform).get<comp::graphic>().texture = &tm.get("stone");
		em.get(newPlatform).get<comp::graphic>().texLocation = IntRect(0, 0, size.x, size.y);
		em.get(newPlatform).get<comp::graphic>().init();
		em.get(newPlatform).get<comp::collider>().setSize(size.x / 2, size.y / 2);
		em.get(newPlatform).get<comp::collider>().setPosition(em.get(newPlatform).get<comp::transform>().position);

		return newPlatform;
	}
}

#endif