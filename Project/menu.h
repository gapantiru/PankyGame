#ifndef MENU_H
#define MENU_H


#include "globals.h"
#include "tile.h"
#include "rectangle.h"
#include "slope.h"
#include "animatedtile.h"
#include "gun.h"

#include <vector>
#include <string>

class Graphics;
class Enemy;
class Player;

struct SDL_Texture;
struct SDL_Rect;
struct Tileset;

class Menu {
public:
	Menu();
	Menu(std::string menuName, Vector2 spawnPoint, Graphics &graphics);
	~Menu();

	void update(int elapsedTime, Graphics &graphics);
	void draw(Graphics &graphics);

	Box getStart() { return this->start; }
	Box getQuit() { return this->quit; }

	void optionM(int n) { this->option = n; }
	int getOption() { return this->option; }

private:
	std::string _menuName;

	void loadMenu(std::string menuName, Graphics &graphics);

	Vector2 _spawnPoint;

	Vector2 _size;
	Vector2 _tileSize;

	SDL_Texture* _backgroundTexture;
	SDL_Texture* _optionsTexture;
	SDL_Texture* _nameTexture;

	std::vector<Tile> _tileList;
	std::vector<Tileset> _tilesets;
	std::vector<Rectangle> _collisionRects;

	std::vector<AnimatedTile> _animatedTileList;
	std::vector<AnimatedTileInfo> _animatedTileInfos;

	Vector2 getTilesetPosition(Tileset tls, int gid, int tileWidth, int tileHeight);

	Box start;
	Box quit;

	int option;


};


#endif // !MENU_H
