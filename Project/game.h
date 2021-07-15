#ifndef GAME_H
#define GAME_H

#include "player.h"
#include "level.h"
#include "enemy.h"
#include "gun.h"
#include "menu.h"
#include "hud.h"
#include "sound.h"
#include "razor.h"

class Graphics;

class Game {
public:
	Game();
	~Game();
private:
	void gameLoop(Graphics &graphics,std::string mapName);
	void menuLoop();

	void draw(Graphics &graphics);
	void update(float elapsedTime,Graphics &graphics);

	void drawMenu(Graphics &graphics);
	void updateMenu(float elapsedTime, Graphics &graphics);


	Player _player;

	Razor _razor;

	Level _level;

	Menu _menu;

	HUD _hud;

	Sound _sound;
	
	bool loop;

	bool colide;

	std::string mapName;
};

#endif
