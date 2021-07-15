#include "razor.h"

namespace razor_constants {
	const float SPEED = .2f;
}

Razor::Razor(){}

Razor::Razor(Graphics &graphics, Vector2 spawnPoint):
	AnimatedSprite(graphics, "char/razor.png", 0, 0, 12, 12, spawnPoint.x, spawnPoint.y, 50),
	_dx(0),
	_dy(0),
	direction(LEFT)
{
	graphics.loadImage("char/razor.png");
	this->setupAnimations();
	this->playAnimation("Spin");
}

void Razor::setupAnimations() {
	this->addAnimation(3,0, 0, "Spin", 16, 16, Vector2(0, 0));
}

void Razor::animationDone(std::string currentAnimation) {}

const float Razor::getX() const {
	return this->_x;
}

const float Razor::getY() const {
	return this->_y;
}

void Razor::moveLeft() {
	this->_dx = -razor_constants::SPEED;
	this->playAnimation("Spin");
	this->direction = LEFT;
}

void Razor::moveRight() {
	this->_dx = razor_constants::SPEED;
	this->playAnimation("Spin");
	this->direction = RIGHT;
}
void Razor::handleTileCollisions(std::vector<Rectangle> &others) {
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

void Razor::update(float elapsedTime) {
	if (this->direction == LEFT)
		this->moveLeft();
	else if (this->direction == RIGHT)
		this->moveRight();

	this->_x += this->_dx * elapsedTime;
	AnimatedSprite::update(elapsedTime);
}

void Razor::draw(Graphics &graphics) {
	AnimatedSprite::draw(graphics, this->_x, this->_y);
}

void Razor::touchPlayer(Player &player) {
	player.gainHealth(-1);
}