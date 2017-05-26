#include <SFML\Graphics.hpp>

#include "Asset.h"
#include "Camera.h"

using namespace std;
using namespace sf;

int myPlayer = -1;
int death = -1;

const Vector2f platformSize = { 100, 32 };
const float platformTop = 0;
const float platformMid = 100;
const float platformDown = 200;


int main()
{
	sf::RenderWindow window(sf::VideoMode(1200, 600), "SFML works!");

	srand(time(NULL));

	View v;

	v.setSize(window.getSize().x, window.getSize().y);
	window.setView(v);
	window.setVerticalSyncEnabled(true);
	window.setMouseCursorVisible(false);

	TextureManager TM;
	TM.add("player", "Pics\\player.png");
	TM.add("stone", "Pics\\stone.png");
	TM.add("fire", "Pics\\fire.png");
	TM.add("coin", "Pics\\coin.png");

	TM.get("stone").setRepeated(true);

	Clock clock;
	Time elapsed;
	elapsed = clock.restart();

	Text t;
	Font f;
	f.loadFromFile("Font\\arial.ttf");
	t.setColor(Color::Red);
	t.setFont(f);
	t.setCharacterSize(35);

	EntityManager EM;

	myPlayer = asset::createPlayer(EM, TM, { 0, 0 }, 10, { 32, 32 });

	asset::createPlatform(EM, TM, { 0, platformMid }, { 1500, platformSize.y });
	death = EM.createEntity();
	EM.get(death).activate();
	EM.get(death).setTag("fire");
	EM.get(death).add<comp::collider>();
	EM.get(death).get<comp::collider>().setPosition(0, 300);
	EM.get(death).get<comp::collider>().setSize(100, 40);


	Camera camera(window, v);
	camera.init();

	int playerBlock = 1;
	int lastBuiltBlock = 1;

	int newSet[] = { 0, 0, 0 };
	int lastSet[] = { 0, 0, 1 };

	bool lastFire = false;

	while (window.isOpen())
	{
		elapsed = clock.restart();

		playerBlock = EM.get(myPlayer).get<comp::transform>().position.x / platformSize.x;


		while (lastBuiltBlock < playerBlock + 35) // need new block
		{
			newSet[0] = 0;
			newSet[1] = 0;
			newSet[2] = 0;

			int randValue = rand() % 10;

			if (lastFire)
			{
				randValue = lastSet[0] ? 3 : (lastSet[1] ? 6 : 9);
			}

			lastFire = false;

			if (randValue < 2 && (lastSet[0] || lastSet[1] || lastSet[2]))
			{
				if (!(rand() % 3))
				{
					if (lastSet[0])
					{
						asset::createCoin(EM, TM, { 100.0f * (lastBuiltBlock + 1), platformTop - 150 });
					}
					else if (lastSet[1])
					{
						asset::createCoin(EM, TM, { 100.0f * (lastBuiltBlock + 1), platformMid - 150 });
					}
					else
					{
						asset::createCoin(EM, TM, { 100.0f * (lastBuiltBlock + 1), platformDown - 150 });
					}

				}

			}
			else if (randValue < 5 && (lastSet[0] || lastSet[1]))
			{
				asset::createPlatform(EM, TM, { platformSize.x * (lastBuiltBlock + 1), 0 }, platformSize);

				if (!(rand() % 8) && lastSet[0])
				{
					lastFire = true;
					asset::createFire(EM, TM, { platformSize.x * (lastBuiltBlock + 1), platformTop - 32 });
				}
				else if (!(rand() % 8))
				{
					asset::createCoin(EM, TM, { platformSize.x * (lastBuiltBlock + 1), platformTop - 50 });
				}

				newSet[0] = 1;
			}
			else if (randValue < 8)
			{
				asset::createPlatform(EM, TM, { platformSize.x * (lastBuiltBlock + 1), 100 }, platformSize);

				if (!(rand() % 8) && lastSet[1])
				{
					lastFire = true;
					asset::createFire(EM, TM, { platformSize.x * (lastBuiltBlock + 1), platformMid - 32 });
				}
				else if (!(rand() % 8))
				{
					asset::createCoin(EM, TM, { platformSize.x * (lastBuiltBlock + 1), platformMid - 50 });
				}

				newSet[1] = 1;
			}
			else
			{
				asset::createPlatform(EM, TM, { platformSize.x * (lastBuiltBlock + 1), 200 }, platformSize);

				if (!(rand() % 8) && lastSet[2])
				{
					lastFire = true;
					asset::createFire(EM, TM, { platformSize.x * (lastBuiltBlock + 1), platformDown - 32 });
				}
				else if (!(rand() % 8))
				{
					asset::createCoin(EM, TM, { platformSize.x * (lastBuiltBlock + 1), platformDown - 50 });
				}

				newSet[2] = 1;
			}

			lastSet[0] = newSet[0];
			lastSet[1] = newSet[1];
			lastSet[2] = newSet[2];
			lastBuiltBlock++;
		}

		t.setString(to_string(EM.get(myPlayer).get<comp::player>().points + playerBlock));

		if (elapsed.asSeconds() > 1 / 60.0f)
		{
			elapsed = seconds(1 / 60.0f);
		}

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == Event::KeyPressed)
			{
				if (Keyboard::isKeyPressed(Keyboard::Escape))
					window.close();
			}
		}

		sys::handleInput(EM, myPlayer, camera.getPosition());
		sys::cleanFarFromPlayer(EM, myPlayer);

		sys::gravity(EM, elapsed.asMilliseconds());

		sys::movement(EM, elapsed.asMilliseconds());

		EM.get(death).get<comp::collider>().setPosition(EM.get(myPlayer).get<comp::transform>().position.x, 300);

		sys::collision(EM);

		if (!EM.get(myPlayer).get<comp::player>().isAlive)
		{
			EM.get(myPlayer).deactivate();
			t.setString("You died\nScore: " + to_string(EM.get(myPlayer).get<comp::player>().points + playerBlock) +
				"\nPress R to restart");
			if (Keyboard::isKeyPressed(Keyboard::R))
			{
				sys::cleanAll(EM);

				myPlayer = asset::createPlayer(EM, TM, { 0, 0 }, 10, { 32, 32 });

				asset::createPlatform(EM, TM, { 0, platformMid }, { 1500, platformSize.y });
				death = EM.createEntity();
				EM.get(death).activate();
				EM.get(death).setTag("fire");
				EM.get(death).add<comp::collider>();
				EM.get(death).get<comp::collider>().setPosition(0, 300);
				EM.get(death).get<comp::collider>().setSize(100, 40);

				playerBlock = 1;
				lastBuiltBlock = 1;

				newSet[0] = 0;
				newSet[1] = 0;
				newSet[2] = 0;

				lastSet[0] = 0;
				lastSet[1] = 0;
				lastSet[2] = 1;
			}
		}

		camera.update(Vector2f(EM.get(myPlayer).get<comp::transform>().position.x, 100));

		t.setPosition(v.getCenter() - (v.getSize() / 2.0f));

		window.clear(Color::Blue);

		sys::display(EM, window);
		camera.drawCursor();
		window.draw(t);

		window.display();
	}

	return 0;
}