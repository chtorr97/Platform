#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <SFML\Graphics.hpp>
#include <memory>

using namespace std;
using namespace sf;

class TextureManager
{
public:
	TextureManager();

	void add(string key, string filePath);
	Texture& get(string key);
	void erase(string key);

private:

	map<string, Texture> texMap;
};

#endif