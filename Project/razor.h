#ifndef RAZOR_H
#define RAZOR_H

#include"animatedsprite.h"
#include "graphics.h"
#include "globals.h"
#include "player.h"

class Graphics;


class Razor :public AnimatedSprite {

public:
	Razor();
	Razor(Graphics &graphics, Vector2 spawnPoint);
	void draw(Graphics &graphics);
	void update(float elapsedTime);

	void moveLeft();

	void moveRight();

	virtual void animationDone(std::string currentAnimation);
	virtual void setupAnimations();

	void handleTileCollisions(std::vector<Rectangle> &others);

	const float getX() const;
	const float getY() const;

	void touchPlayer(Player &player);


private:
	float _dx, _dy;

	Direction _facing;

	Direction direction;

};

#endif

