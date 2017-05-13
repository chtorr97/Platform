#include "Camera.h"

Camera::Camera(RenderWindow& _window, View& _view, Vector2f& _focus)
	: window(_window), view(_view), focus(_focus)
{
	init();
}

void Camera::init()
{
	cursor = VertexArray(Triangles, 3);
	cursor[0].position = sf::Vector2f(0, 0);
	cursor[1].position = sf::Vector2f(SIZE.x, SIZE.y - SIZE.y * 0.4);
	cursor[2].position = sf::Vector2f(SIZE.x - SIZE.x * 0.4, SIZE.y);
	cursorPos = view.getCenter();
	cursor[0].color = color;
	cursor[1].color = Color::Yellow;
	cursor[2].color = Color::Yellow;
	pos = Vector2f(Mouse::getPosition(window).x, Mouse::getPosition(window).y);
	ppos = pos;
	pview = view.getCenter();
	lvlSize = Vector2f(-1, -1);
}

void Camera::update()
{
	view.setCenter(focus); //setView & update window
	if (lvlSize.x != -1 && //check X level limit
		(view.getCenter().x + view.getSize().x / 2.f > lvlSize.x ||
			view.getCenter().x - view.getSize().x / 2.f < 0))
	{
		view.setCenter(pview.x, view.getCenter().y);
	}
	if (lvlSize.y != -1 && //check Y level limit
		(view.getCenter().y + view.getSize().y / 2.f > lvlSize.y ||
			view.getCenter().y - view.getSize().y / 2.f < 0))
	{
		view.setCenter(view.getCenter().x, pview.y);
	}
	window.setView(view);
	cursorPos += (pos - ppos) + (view.getCenter() - pview); //update cursos position
	checkEdges(); //check cursor limit
	ppos = pos;
	pos = Vector2f(Mouse::getPosition(window)); //change/update/store values
	pview = view.getCenter();
	cursor[0].color = color;
}

void Camera::drawCursor()
{
	Transform t;
	t.translate(cursorPos);
	window.draw(cursor, t);
}

void Camera::checkEdges()
{
	if (Mouse::getPosition().x >= window.getSize().x - 1 ||
		Mouse::getPosition().x <= 1 ||
		Mouse::getPosition().y >= window.getSize().y - 1 ||
		Mouse::getPosition().y <= 1)
	{
		Mouse::setPosition((Vector2i)window.getSize() / 2);
		pos = Vector2f(Mouse::getPosition(window));
	}

	//rect dx
	if (cursorPos.x > view.getCenter().x + view.getSize().x / 2.f)
	{
		cursorPos = { view.getCenter().x + view.getSize().x / 2.f, cursorPos.y };
	}
	//rect up
	if (cursorPos.y < view.getCenter().y - view.getSize().y / 2.f)
	{
		cursorPos = { cursorPos.x, view.getCenter().y - view.getSize().y / 2.f };
	}
	//rect sx
	if (cursorPos.x < view.getCenter().x - view.getSize().x / 2.f)
	{
		cursorPos = { view.getCenter().x - view.getSize().x / 2.f, cursorPos.y };
	}
	//rect down
	if (cursorPos.y > view.getCenter().y + view.getSize().y / 2.f)
	{
		cursorPos = { cursorPos.x, view.getCenter().y + view.getSize().y / 2.f };
	}
}