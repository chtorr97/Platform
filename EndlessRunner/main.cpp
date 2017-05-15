#include <SFML\Graphics.hpp>

#include "Asset.h"
#include "Camera.h"

using namespace std;
using namespace sf;

int myPlayer = -1;

int main()
{
	sf::RenderWindow window(sf::VideoMode(1200, 600), "SFML works!");

	View v;

	v.setSize(window.getSize().x, window.getSize().y);
	window.setView(v);
	window.setVerticalSyncEnabled(true);
	window.setMouseCursorVisible(false);

	TextureManager TM;
	TM.add("player", "Pics\\player.png");
	TM.add("stone", "Pics\\stone.png");
	TM.add("crate", "Pics\\crate.png");

	TM.get("stone").setRepeated(true);

	Clock clock;
	Time elapsed;
	elapsed = clock.restart();

	Text t;
	Font f;
	f.loadFromFile("Font\\arial.ttf");
	t.setColor(Color::Red);
	t.setFont(f);
	t.setCharacterSize(16);

	EntityManager EM;

	myPlayer = asset::createPlayer(EM, TM, { 130, 50 }, 10, { 32, 32 });

	int crate = EM.createEntity();
	EM.get(crate).activate();
	EM.get(crate).add<comp::transform>();
	EM.get(crate).add<comp::physic>();
	EM.get(crate).add<comp::graphic>();
	EM.get(crate).add<comp::collider>();
	EM.get(crate).get<comp::transform>().position = { 130, 50 };
	EM.get(crate).get<comp::physic>().mass = 10;
	EM.get(crate).get<comp::graphic>().texture = &TM.get("crate");
	EM.get(crate).get<comp::graphic>().texLocation = IntRect(0, 0, 32, 32);
	EM.get(crate).get<comp::graphic>().init();
	EM.get(crate).get<comp::collider>().setSize(16, 16);
	EM.get(crate).get<comp::collider>().setPosition(EM.get(crate).get<comp::transform>().position);
	/*
	EM.get(player).get<comp::animation>().addAnimations();
	EM.get(player).get<comp::animation>().addFrame(100);
	EM.get(player).get<comp::animation>().addFrame(100);
	EM.get(player).get<comp::animation>().addFrame(100);
	EM.get(player).get<comp::animation>().addFrame(100);
	EM.get(player).get<comp::animation>().addFrame(100);
	EM.get(player).get<comp::animation>().addFrame(100);
	EM.get(player).get<comp::animation>().loop(0);*/

	for (int k = 0; k < 5; k++)
	{
		asset::createPlatform(EM, TM, { 150 + k * 100.f, 150 + k * 100.f }, 10, { 500, 50 });
		/*roccia = EM.createEntity();
		cout << roccia << endl << myPlayer << endl;
		EM.get(roccia).activate();
		EM.get(roccia).add<comp::transform>();
		EM.get(roccia).add<comp::physic>();
		EM.get(roccia).add<comp::graphic>();
		EM.get(roccia).add<comp::collider>();
		EM.get(roccia).get<comp::transform>().position = { 150 + k * 100.f, 150 + k * 100.f };
		EM.get(roccia).get<comp::physic>().mass = 10;
		EM.get(roccia).get<comp::physic>().isKinematic = true;
		EM.get(roccia).get<comp::graphic>().texture = &tm.get("stone");
		EM.get(roccia).get<comp::graphic>().texLocation = IntRect(0, 0, 64, 32);
		EM.get(roccia).get<comp::graphic>().init();
		EM.get(roccia).get<comp::collider>().setSize(32, 16);
		EM.get(roccia).get<comp::collider>().setPosition(EM.get(roccia).get<comp::transform>().position);*/
	}

	Camera camera(window, v, EM.get(myPlayer).get<comp::transform>().position);
	camera.init();

	while (window.isOpen())
	{
		elapsed = clock.restart();

		if (elapsed.asSeconds() > 1 / 20.0f)
		{
			elapsed = seconds(1 / 20.0f);
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

		sys::movement(EM, elapsed.asMilliseconds());
		sys::gravity(EM, elapsed.asMilliseconds());
		sys::collision(EM);

		camera.update();

		window.clear();

		sys::display(EM, window);
		camera.drawCursor();

		window.display();
	}

	return 0;
}