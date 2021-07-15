#include "enemy.h"

//base enemy class

namespace enemy_constants {
	
	const int DOWN = 450;

	const float GRAVITY = 0.01f;
	const float GRAVITY_CAP = 0.2f;

}
//_____________________________ENEMY CLASS________________________________________//
Enemy::Enemy(){}

Enemy::Enemy(Graphics &graphics, std::string filePath, int sourceX, int sourceY, int width, int height,
	Vector2 spawnPoint, int timeToUpdate) :
	AnimatedSprite(graphics, filePath, sourceX, sourceY, width, height,
		spawnPoint.x, spawnPoint.y, timeToUpdate),
	_direction(LEFT),
	direction(LEFT),
	_maxHealth(2),
	_currentHealth(1),
	isDown(false),
	_dx(0),
	_dy(0)
{}

void Enemy::update(int elapsedTime, Player &player) {

	//apply gravity
	//if (this->_dy <= enemy_constants::GRAVITY_CAP) {
	//	this->_dy += enemy_constants::GRAVITY * elapsedTime;
	//}

	////Move by dx
	//this->_x += this->_dx * elapsedTime;

	////move by dy
	//this->_y += this->_dy * elapsedTime;

	if (this->direction == LEFT) {
		if (this->_dx <= enemy_constants::GRAVITY_CAP)
			this->_dx += enemy_constants::GRAVITY*elapsedTime;
		this->_x += this->_dx*elapsedTime;
	}
	else if (this->direction == RIGHT) {
		if (this->_dx <= enemy_constants::GRAVITY_CAP)
			this->_dx -= enemy_constants::GRAVITY*elapsedTime;
		this->_x -= this->_dx*elapsedTime;
	}
	
	AnimatedSprite::update(elapsedTime);
}

void Enemy::draw(Graphics &graphics) {
	AnimatedSprite::draw(graphics, this->_x, this->_y);
}

void Enemy::dropHealth() {
	if (this->_currentHealth > 0)
		this->_currentHealth += -1;
}


void Enemy::handleTileCollisions(std::vector<Rectangle> &others) {
	//figure out what side the collision happened and move the player accordinlgy
	for (int i = 0; i < others.size(); i++) {
		sides::Side collisionSide = Sprite::getCollisionSide(others.at(i));
		if (collisionSide != sides::NONE) {
			switch (collisionSide) {
			case sides::LEFT:
				this->direction = LEFT;
				break;
			case sides::RIGHT:
				this->direction = RIGHT;
				break;
			case sides::TOP:
				this->_y = others.at(i).getBottom() + 1;
				this->_dy = 0;
				break;
			case sides::BOTTOM:
				this->_y = others.at(i).getTop() - this->_boundingBox.getHeight() - 1;
				this->_dy = 0;
				break;

			}

		}
	}
}

//_________________________________________________BAT CLASS___________________________________________//
Bat::Bat(){}

Bat::Bat(Graphics &graphics, Vector2 spawnPoint) :
	Enemy(graphics, "char/red.png", 32, 32, 16, 16, spawnPoint, 140),
	_startingX(spawnPoint.x),
	_startingY(spawnPoint.y),
	_shouldMoveUp(false)
{
	this->setupAnimations();
	this->playAnimation("FlyLeft");
}

void Bat::update(int elapsedTime, Player &player) {
	this->_direction = player.getX() > this->_x ? RIGHT : LEFT;
	this->playAnimation(this->_direction == RIGHT ? "FlyRight" : "FlyLeft");

	//move up or down
	this->_y += _shouldMoveUp ? -.01 : .01;
	if (this->_y > (_startingY + 50) || this->_y < (this->_startingY - 10)) {
		this->_shouldMoveUp = !this->_shouldMoveUp;
	}

	Enemy::update(elapsedTime, player);
}

void Bat::draw(Graphics &graphics) {
	Enemy::draw(graphics);
}

void Bat::animationDone(std::string currentAnimation) {

}

void Bat::setupAnimations() {
	this->addAnimation(3, 2, 32, "FlyLeft", 16, 16, Vector2(0, 0));
	this->addAnimation(3, 2, 48, "FlyRight", 16, 16, Vector2(0, 0));

}

void Bat::touchPlayer(Player* player) {
	player->gainHealth(-1);
}

void Bat::dropHealth() {
	Enemy::dropHealth();
}

