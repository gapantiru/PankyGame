#ifndef GUN_H
#define GUN_H

#include "player.h"
#include "sprite.h"

class Gun:public Sprite {
public:
	Gun();
	Gun(Graphics &graphics, const std::string filePath, int sourceX, int sourceY, int width, int height, Vector2 spawnPoint);
	
	virtual void update();
	virtual void draw(Graphics &graphics) = 0;

	virtual bool getStop()=0;

	virtual ~Gun();


};

class Projectile:public Gun {
public:
	Projectile();
	Projectile(Graphics &graphics, Vector2 spawnPoint,Direction direction);
	void update();
	void draw(Graphics &graphics);
	void startQ();

	float getY();

	bool getStop();

	~Projectile();


private:
	float _startX, _startY;
	float _dx, _dy;
	bool stop;

	Direction _direction;
};

class Bullet :public Gun {
public:
	Bullet();
	Bullet(Graphics &graphics, Vector2 spawnPoint,float x,float y);
	void update();
	void draw(Graphics &graphics);
	void startQ();

	float getY();
	float getX();

	bool getStop();

	~Bullet();

private:
	float startX, startY;
	float dx, dy;
	bool stop;
	float goX, goY;



};

#endif // !GUN_H
