#include "car.h"

namespace car_constants {
	const float SPEED = .2f;
}



Car::Car(){}

Car::Car(Graphics &graphics, Vector2 spawnPoint) :
	Sprite(graphics, "char/movingTile.png", 0, 0, 32, 8, spawnPoint.x, spawnPoint.y),
	_dx(0),
	_dy(0),
	direction(LEFT)
{

}

const float Car::getX() const {
	return this->_x;
}

const float Car::getY() const {
	return this->_y;
}

void Car::moveLeft() {
	this->_dx = -car_constants::SPEED;
	this->direction = LEFT;
}

void Car::moveRight() {
	this->_dx = car_constants::SPEED;
	this->direction = RIGHT;
}

void Car::handleTileCollisions(std::vector<Rectangle> &others) {
	//figure out what side the collision happened and move the player accordinlgy
	for (int i = 0; i < others.size(); i++) {
		sides::Side collisionSide = Sprite::getCollisionSide(others.at(i));
		if (collisionSide != sides::NONE) {
			switch (collisionSide) {
			case sides::LEFT:
				this->direction = RIGHT;
				break;
			case sides::RIGHT:
				this->direction = LEFT;
				break;
			}

		}
	}
}

void Car::update(float elapsedTime) {
	if (this->direction == LEFT)
		this->moveLeft();
	else if (this->direction == RIGHT)
		this->moveRight();

	this->_x += this->_dx * elapsedTime;
	Sprite::update();
}

void Car::draw(Graphics &graphics) {
	Sprite::draw(graphics, this->_x, this->_y);
}
