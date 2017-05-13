#include <SFML\Graphics.hpp>

#include "MyECS\Systems.h"
#include "TextureManager.h"
#include "Camera.h"
#include "Obstacles.h"

using namespace std;
using namespace sf;

int player = -1;

int main()
{
	sf::RenderWindow window(sf::VideoMode(1200, 600), "SFML works!");

	View v;

	v.setSize(window.getSize().x, window.getSize().y);
	window.setView(v);
	window.setVerticalSyncEnabled(true);
	window.setMouseCursorVisible(false);

	TextureManager tm;
	tm.add("player", "Pics\\player.png");

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

	player = EM.createEntity();
	EM.get(player).activate();
	EM.get(player).add<comp::transform>();
	EM.get(player).add<comp::physic>();
	EM.get(player).add<comp::graphic>();
	EM.get(player).add<comp::animation>();
	EM.get(player).get<comp::transform>().position = { 50, 50 };
	EM.get(player).get<comp::physic>().mass = 10;
	EM.get(player).get<comp::graphic>().texture = &tm.get("player");
	EM.get(player).get<comp::graphic>().texLocation = IntRect(0, 0, 32, 32);
	EM.get(player).get<comp::graphic>().init();
	EM.get(player).get<comp::animation>().addAnimations();
	EM.get(player).get<comp::animation>().addFrame(100);
	EM.get(player).get<comp::animation>().addFrame(100);
	EM.get(player).get<comp::animation>().addFrame(100);
	EM.get(player).get<comp::animation>().addFrame(100);
	EM.get(player).get<comp::animation>().addFrame(100);
	EM.get(player).get<comp::animation>().addFrame(100);
	EM.get(player).get<comp::animation>().loop(0);

	Obstacles obs;

	Camera camera(window, v, EM.get(player).get<comp::transform>().position);
	camera.init();

	/*while (window.isOpen())
	{
		elapsed = clock.restart();

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

		sys::handleInput(EM, player, camera.getPosition());

		sys::movement(EM, elapsed.asMilliseconds());

		camera.update();

		window.clear();

		sys::display(EM, window);
		camera.drawCursor();

		window.display();
	}*/

	return 0;
}