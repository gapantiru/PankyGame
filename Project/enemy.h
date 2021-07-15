#ifndef ENEMY_H
#define ENEMY_H

#include "animatedsprite.h"
#include "graphics.h"
#include"globals.h"
#include"player.h"

#include<string>

class Graphics;
class Player;

class Enemy : public AnimatedSprite {
public:
	Enemy();
	Enemy(Graphics &graphics, std::string filePath, int sourceX, int sourceY,
		int width, int height, Vector2 spawnPoint, int timeToUpdate);
	virtual void update(int elapsedTime, Player &player);
	virtual void draw(Graphics &graphics);
	virtual void touchPlayer(Player* player) = 0;

	const inline int getMaxHealth() const { return this->_maxHealth; }
	const inline int getCurrentHealth() const { return this->_currentHealth; }
	virtual void dropHealth();

	virtual void Enemy::handleTileCollisions(std::vector<Rectangle> &others);

	virtual float getX() { return this->_x; }
	virtual float getY() { return this->_y; }
	

protected:
	Direction _direction;

	Direction direction;

	int _maxHealth;
	int _currentHealth;

	bool isDown;

private:
	float _dx, _dy;
};

class Bat : public Enemy {
public:
	Bat();
	Bat(Graphics &graphics, Vector2 spawnPoint);
	void update(int elapsedTime, Player &player);
	void draw(Graphics &graphics);
	void touchPlayer(Player* player);

	void dropHealth();

	void animationDone(std::string currentAnimation);
	void setupAnimations();

	float getX() { return this->_x; }
	float getY() { return this->_y; }


private:
	float _startingX, _startingY;
	bool _shouldMoveUp;
};


#endif