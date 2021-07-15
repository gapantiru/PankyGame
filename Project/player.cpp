#include "player.h"
#include "graphics.h"

namespace player_constants {
	const float WALK_SPEED = 0.2f;
	const float JUMP_SPEED = 0.7f;

	const float GRAVITY = 0.002f;
	const float GRAVITY_CAP = 0.8f;

	const float CAR_SPEED = 0.2f;
}

Player::Player() {};

Player::Player(Graphics &graphics, Vector2 spawnPoint) :
	AnimatedSprite(graphics, "char/finalSkull.png", 0, 0, 10, 15, spawnPoint.x, spawnPoint.y, 100),
	_dx(0),
	_dy(0),
	_dxx(0),
	_facing(RIGHT),
	_grounded(false),
	_maxHealth(3),
	_currentHealth(3),
	_gain(true),
	_onCar(false)
	{
	graphics.loadImage("char/finalSkull.png");
	this->setupAnimations();
	this->playAnimation("RunRight");

	}

void Player::setupAnimations() {
	this->addAnimation(1, 0, 0, "IdleRight", 10, 17, Vector2(0, 0));
	this->addAnimation(1, 0, 17, "IdleLeft", 10, 17, Vector2(0, 0));
	this->addAnimation(3, 0, 0, "RunRight", 10, 17, Vector2(0, 0));
	this->addAnimation(3, 0, 17, "RunLeft", 10, 17, Vector2(0, 0));

}

void Player::animationDone(std::string currentAnimation) {}

const float Player::getX() const {
	return this->_x;
}

const float Player::getY() const {
	return this->_y;
}

void Player::moveLeft() {
	this->_dx = -player_constants::WALK_SPEED;
	this->playAnimation("RunLeft");
	this->_facing = LEFT;
}

void Player::moveRight() {
	this->_dx = player_constants::WALK_SPEED;
	this->playAnimation("RunRight");
	this->_facing = RIGHT;
}

void Player::stopMoving() {
		this->_dx = 0.0f;
	this->playAnimation(this->_facing == RIGHT ? "IdleRight" : "IdleLeft");
}

void Player::jump() {
	if (this->_grounded) {
		this->_dy = 0;
		this->_dy -= player_constants::JUMP_SPEED;
		this->_grounded = false;
	}
}

void Player::moveUp() {
	this->_y += -1;
}

//void handleTileCollision
//handles collision with all tiles the player is colliding with
void Player::handleTileCollisions(std::vector<Rectangle> &others) {
	//figure out what side the collision happened and move the player accordinlgy
	for (int i = 0; i < others.size(); i++) {
		sides::Side collisionSide = Sprite::getCollisionSide(others.at(i));
		if (collisionSide != sides::NONE) {
			switch (collisionSide) {
			case sides::TOP:
				this->_y = others.at(i).getBottom() + 1;
				this->_dy = 0;
				break;
			case sides::BOTTOM:
				this->_y = others.at(i).getTop() - this->_boundingBox.getHeight() - 1;
				this->_dy = 0;
				this->_grounded = true;
				break;
			case sides::LEFT:
				this->_x = others.at(i).getRight() + 1;
				break;
			case sides::RIGHT:
				this->_x = others.at(i).getLeft() - this->_boundingBox.getWidth() - 1;
				break;
			}

		}
	}
}
void Player::handleCarCollisions(std::vector<Car*> &others, float elapsedTime) {
	//figure out what side the collision happened and move the player accordinlgy
	for (int i = 0; i < others.size(); i++) {
		if (others.at(i)->getBoundingBox().collidesWith(this->getBoundingBox())) {
			Rectangle other = others.at(i)->getBoundingBox();
			sides::Side collisionSide = Sprite::getCollisionSide(other);
			if (collisionSide != sides::NONE) {
				switch (collisionSide) {
				case sides::TOP:
					this->_y = others.at(i)->getBoundingBox().getBottom() + 1;
					this->_dy = 0;
					break;
				case sides::BOTTOM:
					this->_y = others.at(i)->getBoundingBox().getTop() - this->_boundingBox.getHeight() - 1;
					this->_dy = 0;
					//if (others.at(i)->getDirection() == LEFT)
					//	this->_x += -player_constants::CAR_SPEED * elapsedTime ;
					//else if (others.at(i)->getDirection() == RIGHT)
					//	this->_x += player_constants::CAR_SPEED * elapsedTime ;
					this->_grounded = true;

					break;
				case sides::LEFT:
					this->_x = others.at(i)->getBoundingBox().getRight() + 1;
					break;
				case sides::RIGHT:
					this->_x = others.at(i)->getBoundingBox().getLeft() - this->_boundingBox.getWidth() - 1;
					break;
				}

			}
		}
	}
}


//handles collision with all slopes the player is colliding with
void Player::handleSlopeCollisions(std::vector<Slope> &others) {
	for (int i = 0; i < others.size(); i++) {
		//calculate where on the slope the player's bottom center is touching 
		//and use y=mx+b to figure out the y position to place him at
		//first calculate "b" (slope intercept) using one of the points (b = y - mx)
		int b = (others.at(i).getP1().y - (others.at(i).getSlope() * fabs(others.at(i).getP1().x)));

		//new we need to get player's center x
		int centerX = this->_boundingBox.getCenterX();

		//now pass that x into equation y = mx + b (using our newly found b and x) to get the new y position
		int newY = (others.at(i).getSlope() * centerX) + b - 8; //8 its temporary to fix a problem

		//re-position the player to the correct "y"
		if (this->_grounded) {
			this->_y = newY - this->_boundingBox.getHeight();
			this->_grounded = true;
		}

	}
}

void Player::handleEnemyCollsions(std::vector<Enemy*> &others) {
	for (int i = 0; i < others.size(); i++) {
		others.at(i)->touchPlayer(this);
	}
}

void Player::gainHealth(int amount) {
	if (this->_currentHealth > 0)
		this->_currentHealth += amount;
}

void Player::update(float elapsedTime) {
	//apply gravity
	if (this->_dy <= player_constants::GRAVITY_CAP) {
		this->_dy += player_constants::GRAVITY * elapsedTime;
	}

	//Move by dx
	this->_x += this->_dx * elapsedTime;

	//move by dy
	this->_y += this->_dy * elapsedTime;


	AnimatedSprite::update(elapsedTime);

}

void Player::draw(Graphics &graphics) {
	AnimatedSprite::draw(graphics, this->_x, this->_y);

}