#ifndef ASSET_H
#define ASSET_H

#include "MyECS\Systems.h"
#include "TextureManager.h"

namespace asset
{
	int createPlayer(EntityManager& em, TextureManager& tm, Vector2f pos = { 0, 0 }, float mass = 10, Vector2f size = { 32, 32 })
	{
		int newPlayer = em.createEntity();

		em.get(newPlayer).activate();
		em.get(newPlayer).setTag("player");
		em.get(newPlayer).add<comp::player>();
		em.get(newPlayer).add<comp::transform>();
		em.get(newPlayer).add<comp::physic>();
		em.get(newPlayer).add<comp::graphic>();
		em.get(newPlayer).add<comp::collider>();
		em.get(newPlayer).get<comp::transform>().position = pos;
		em.get(newPlayer).get<comp::physic>().mass = mass;
		em.get(newPlayer).get<comp::physic>().maxSpeed = 3;
		em.get(newPlayer).get<comp::graphic>().texture = &tm.get("player");
		em.get(newPlayer).get<comp::graphic>().texLocation = IntRect(0, 0, size.x, size.y);
		em.get(newPlayer).get<comp::graphic>().init();
		em.get(newPlayer).get<comp::collider>().setSize(size);
		em.get(newPlayer).get<comp::collider>().setPosition(em.get(newPlayer).get<comp::transform>().position);

		return newPlayer;
	}

	int createPlatform(EntityManager& em, TextureManager& tm, Vector2f pos = { 0, 0 }, Vector2f size = { 32, 32 })
	{
		int newPlatform = em.createEntity();

		em.get(newPlatform).activate();
		em.get(newPlatform).setTag("platform");
		em.get(newPlatform).add<comp::transform>();
		em.get(newPlatform).add<comp::graphic>();
		em.get(newPlatform).add<comp::physic>();
		em.get(newPlatform).add<comp::collider>();
		em.get(newPlatform).get<comp::transform>().position = pos;
		em.get(newPlatform).get<comp::physic>().isKinematic = true;
		em.get(newPlatform).get<comp::graphic>().texture = &tm.get("stone");
		em.get(newPlatform).get<comp::graphic>().texLocation = IntRect(0, 0, size.x, size.y);
		em.get(newPlatform).get<comp::graphic>().init();
		em.get(newPlatform).get<comp::collider>().setSize(size);
		em.get(newPlatform).get<comp::collider>().setPosition(em.get(newPlatform).get<comp::transform>().position);

		return newPlatform;
	}

	int createFire(EntityManager& em, TextureManager& tm, Vector2f pos = { 0, 0 }, Vector2f size = { 32, 32 })
	{
		int newFire = em.createEntity();

		em.get(newFire).activate();
		em.get(newFire).setTag("fire");
		em.get(newFire).add<comp::transform>();
		em.get(newFire).add<comp::animation>();
		em.get(newFire).add<comp::physic>();
		em.get(newFire).add<comp::graphic>();
		em.get(newFire).add<comp::collider>();
		em.get(newFire).get<comp::transform>().position = pos;
		em.get(newFire).get<comp::physic>().isKinematic = true;
		em.get(newFire).get<comp::graphic>().texture = &tm.get("fire");
		em.get(newFire).get<comp::graphic>().texLocation = IntRect(0, 0, size.x, size.y);
		em.get(newFire).get<comp::graphic>().init();
		em.get(newFire).get<comp::collider>().setSize(size);
		em.get(newFire).get<comp::collider>().setPosition(em.get(newFire).get<comp::transform>().position);
		em.get(newFire).get<comp::animation>().addAnimations();
		float speed = 5;
		for (int k = 0; k < 4; k++)
			em.get(newFire).get<comp::animation>().addFrame(speed);
		em.get(newFire).get<comp::animation>().loop(0);

		return newFire;
	}

	int createCoin(EntityManager& em, TextureManager& tm, Vector2f pos = { 0, 0 })
	{
		int newCoin = em.createEntity();

		em.get(newCoin).activate();
		em.get(newCoin).setTag("coin");
		em.get(newCoin).add<comp::transform>();
		em.get(newCoin).add<comp::physic>();
		em.get(newCoin).add<comp::animation>();
		em.get(newCoin).add<comp::graphic>();
		em.get(newCoin).add<comp::collider>();
		em.get(newCoin).get<comp::transform>().position = pos;
		em.get(newCoin).get<comp::physic>().mass = 10;
		em.get(newCoin).get<comp::physic>().isKinematic = true;
		em.get(newCoin).get<comp::graphic>().texture = &tm.get("coin");
		em.get(newCoin).get<comp::graphic>().texLocation = IntRect(0, 0, 32, 32);
		em.get(newCoin).get<comp::graphic>().init();
		em.get(newCoin).get<comp::collider>().setSize(32, 32);
		em.get(newCoin).get<comp::collider>().setPosition(em.get(newCoin).get<comp::transform>().position);
		em.get(newCoin).get<comp::animation>().addAnimations();
		float speed = 5;
		for (int k = 0; k < 10; k++)
			em.get(newCoin).get<comp::animation>().addFrame(speed);
		em.get(newCoin).get<comp::animation>().loop(0);

		return newCoin;
	}
}

#endif