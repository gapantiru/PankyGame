#include"gun.h"
#include"player.h"
#include"enemy.h"
#include"graphics.h"

namespace gun_constants {
	const float SPEED = .7f;
	const float P_SPEED = .45f;
	const float B_SPEED = .7f;

}

Gun::Gun() {}

Gun::Gun(Graphics &graphics, std::string filePath, int sourceX, int sourceY, int width, int height, Vector2 spawnPoint) :
	Sprite(graphics, filePath, sourceX, sourceY, width, height, spawnPoint.x, spawnPoint.y)
{

}

void Gun::update() {
	Sprite::update();
}


void Gun::draw(Graphics &graphics) {
	Sprite::draw(graphics, this->_x, this->_y);
}


Gun::~Gun() {
	Sprite::~Sprite();
}


Projectile::Projectile(){}

Projectile::Projectile(Graphics& graphics, Vector2 spawnPoint,Direction direction) :
	Gun(graphics, "char/Bullet.png", 202, 2, 4, 4, spawnPoint),
	_dx(spawnPoint.x),
	_dy(spawnPoint.y),
	stop(false),
	_direction(direction)
{
	Sprite::update();
}

void Projectile::update() {
	
	if (this->_y < 0)
		this->stop = true;
	else {
		if (this->_direction == UP)
			this->_y += -1 * gun_constants::P_SPEED;
		else if (this->_direction == LEFT)
			this->_x += -1 * gun_constants::P_SPEED;
		else if (this->_direction == RIGHT)
			this->_x += 1 * gun_constants::P_SPEED;
		Gun::update();
	}
}


void Projectile::draw(Graphics &graphics) {
	Gun::draw(graphics);
}

void Projectile::startQ() {
	this->stop = true;
}

bool Projectile::getStop() {
	return this->stop;
}

Projectile::~Projectile() {
	Gun::~Gun();
}

//BULLET CLASS

Bullet::Bullet(){}

Bullet::Bullet(Graphics &graphics, Vector2 spawnPoint,float x,float y) :
	Gun(graphics, "char/red.png", 97, 18, 5, 5, spawnPoint),
	dx(1),
	dy(1),
	startX(spawnPoint.x),
	startY(spawnPoint.y),
	stop(false),
	goX(x),
	goY(y)
{
	Sprite::update();
}

void Bullet::update() {
	/*
	float x = (this->goX+5);
	float y = this->goY;
	double m = (this->startY - y) / (this->startX - x);
	if (this->startY < goY)
		this->_y += gun_constants::B_SPEED * gun_constants::SPEED;
	else if (startY > goY)
		this->_y += -gun_constants::B_SPEED * gun_constants::SPEED;
	this->_x = ((this->_y - y) / m) + x;
	if (this->_y > globals::SCREEN_HEIGHT || this->_x<0 || this->_x>globals::SCREEN_WIDTH)
		this->stop = true;
		*/
	Gun::update();
	float x = (this->goX + 5);
	float y = (this->goY + 5);
	float t = 0.003f;
	Vector2 AB(x-startX,y-startY);
	this->_x += t*AB.x;
	this->_y += t*AB.y;
	if (this->_y > globals::SCREEN_HEIGHT || this->_x<0 || this->_x>globals::SCREEN_WIDTH)
		this->stop = true;

}

void Bullet::draw(Graphics &graphics) {
	Gun::draw(graphics);
}

void Bullet::startQ() {
	this->stop = true;
}

bool Bullet::getStop() {
	return this->stop;
}

Bullet::~Bullet() {
	Gun::~Gun();
}
