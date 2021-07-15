#ifndef PLAYER_H
#define PLAYER_H

#include "animatedsprite.h"
#include "globals.h"
#include "slope.h"
#include"enemy.h"
#include "car.h"

class Graphics;
class Enemy;
class Car;

class Player : public AnimatedSprite {
public:
	Player();
	Player(Graphics &graphics,Vector2 spawnPoint);
	void draw(Graphics &graphics);
	void update(float elapsedTime);

	//moves the player by -dx / dx

	void moveLeft();

	void moveRight();

	void stopMoving();

	void moveUp();

	void jump();

	virtual void animationDone(std::string currentAnimation);
	virtual void setupAnimations();

	void handleTileCollisions(std::vector<Rectangle> &others);
	void handleCarCollisions(std::vector<Car*> &others,float elapsedTime);
	void handleSlopeCollisions(std::vector<Slope> &others);
	void handleEnemyCollsions(std::vector<Enemy*> &others);

	const float getX() const;
	const float getY() const;

	void doY(float y) {
		this->_y = y - 1;
		this->_dy = 0;
	}

	const inline int getHealth() const { return this->_maxHealth; }
	const inline int getCurrentHealth()const { return this->_currentHealth; }

	const inline bool getGain() { return this->_gain; }
	virtual void doGain(bool gain) { this->_gain = gain; }

	const inline Direction getFacing() const { return this->_facing; }

	void false_ddx() { this->_dxx = 0.0f; }


	void gainHealth(int amount);

private:
	float _dx, _dy;

	float _dxx;

	Direction _facing;

	bool _grounded;

	int _maxHealth;
	int _currentHealth;

	bool _gain;

	bool _onCar;
};

#endif

