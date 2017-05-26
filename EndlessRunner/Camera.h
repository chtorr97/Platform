#ifndef CAMERA_H
#define CAMERA_H

#include <SFML\Graphics.hpp>

using namespace sf;

class Camera
{
public:
	Camera(RenderWindow& _window, View& _view);

	void init();

	void update(Vector2f& focus);
	void drawCursor();

	const inline Vector2f& getPosition() { return cursorPos; }
	inline void setLvlSize(Vector2f _lvlSize) { lvlSize = _lvlSize; }
	inline void setColor(Color c) { color = c; }

private:
	VertexArray cursor;
	Vector2f cursorPos;
	View& view;
	RenderWindow& window;

	Vector2f pos,
		ppos,
		pview,
		lvlSize;

	const Vector2f SIZE = Vector2f(15, 15);
	Color color = Color::Red;

	void checkEdges();
};

#endif