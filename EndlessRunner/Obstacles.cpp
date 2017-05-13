#include "Obstacles.h"

Obstacles::Obstacles()
{
	obstacles.set_capacity(15);
}

Obstacles::~Obstacles()
{
}

bool Obstacles::checkCollision(IntRect& player)
{
	for (RectangleShape& rs : obstacles)
	{
		//if(rs.)
	}
	return false;
}

void Obstacles::add()
{

}

void Obstacles::draw(RenderWindow& window)
{
	for (RectangleShape& rs : obstacles)
	{
		window.draw(rs);
	}
}