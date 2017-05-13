#ifndef OBSTACLES_H
#define OBSTACLES_H

#include <vector>
#include <SFML\Graphics.hpp>
#include <boost\circular_buffer.hpp>

using namespace std;
using namespace sf;

class Obstacles
{
public:
	Obstacles();
	~Obstacles();

	bool checkCollision(IntRect& player);

	void add();

	void draw(RenderWindow& window);

private:
	boost::circular_buffer<RectangleShape> obstacles;
};

#endif