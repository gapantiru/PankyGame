#ifndef HUD_H
#define HUD_H

#include "sprite.h"
#include "player.h"

class Graphics;

class HUD:public Sprite {
public:
	HUD();
	HUD(Graphics &grahics,Player &player);
	void update(int elapsedTime,int health);
	void draw(Graphics &graphics);

private:
	Player _player;

	Sprite _healthBarSprite;
	Sprite _healthNumber1;
};



#endif